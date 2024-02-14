/**
 * File Name: Wordle.cpp
 * Description: This file implements the Wordle class.
 * Author: Amy Uccello
 * Student Number: 251236036
 * Assignment Number: 1
 * Class: CS3307
 * Date Created: September 30, 2023
*/

#include "Wordle.h"

/**
 * Function: Wordle constructor
 * Description: Initializes a Wordle application
 * Parameters: Wt::WEnvironment
 * Return Value: Wordle application
*/
Wordle::Wordle(const Wt::WEnvironment& env)
  : WApplication(env)
{
  
  guessNum_ = 0; // Initialize the guess number to 0
  
  this->instance()->useStyleSheet("style.css"); // use the style sheet style.css
  
  // Build the allWords_ vector
  std::ifstream myfile;
  myfile.open("words.txt");
  std::string myline;
  
  if ( myfile.is_open() ) {
    while ( myfile ) { // equivalent to myfile.good()
      std::getline (myfile, myline);
      allWords_.push_back(myline);
    }
  }
  else {
    std::cout << "Couldn't open file\n";
  }

  // Generate a random number to pick a word from the allWords_ vector
  srand((unsigned) time(NULL));
  int random = 0 + (rand() % allWords_.size());
  while (usedWordIdxs_.count(random) == 1) {
    random = 0 + (rand() % allWords_.size());
  }
  
  usedWordIdxs_.insert(random); // Ensure the word is not used again in the same session
  
  // Set the current answer word and its characters
  curAnswer_ = boost::to_upper_copy<std::string>(allWords_.at(random));
  for(char& c : curAnswer_) {
    curAnswerChars_.insert(c);
  }
  
  // Build the UI
  // Set the browser tab title
  std::string title = "Wordle!";
  setTitle(title);  

  // Set the title of the game                           
  root()->addWidget(std::make_unique<Wt::WText>("C++ Wordle!"))->setStyleClass("title"); // show some text
  root()->addWidget(std::make_unique<Wt::WBreak>()); 

  // Build the board
  curBoard_ = root()->addWidget(std::make_unique<Wt::WTable>()); 
  for (auto i = 0; i < 6; ++i) {
    for (auto j = 0; j < 5; ++j){
      curState_[i][j] = curBoard_->elementAt(i,j)->addWidget(std::make_unique<Wt::WText>("|_|"));
    }
  }

  // Build the edit box to type in guesses
  nameEdit_ = root()->addWidget(std::make_unique<Wt::WLineEdit>()); // allow text input
  nameEdit_->setInputMask(">AAAAA;_");                  // only upper case
  nameEdit_->setMaxLength(5); 
  nameEdit_->setStyleClass("edit");
  nameEdit_->setPlaceholderText("Enter Word");                        
  nameEdit_->setFocus();                              // give focus

  // Build the submit button
  buttonSubmit_ = root()->addWidget(std::make_unique<Wt::WPushButton>("Submit"));
  buttonSubmit_->setMargin(5, Wt::Side::Left);
  buttonSubmit_->disable();  
  
  // Build the game messages
  root()->addWidget(std::make_unique<Wt::WBreak>());    // insert a line break
  gameMessage_ = root()->addWidget(std::make_unique<Wt::WText>()); // empty text
  gameMessage_->setStyleClass("message");

  root()->addWidget(std::make_unique<Wt::WBreak>());
  answerWordMessage_ = root()->addWidget(std::make_unique<Wt::WText>());
  answerWordMessage_->setStyleClass("message");
  answerWordMessage_->hide(); // hide the answer word message until the game is over

  // Build the new game anchor and make it look like a button
  root()->addWidget(std::make_unique<Wt::WBreak>());    
  linkNewGame_ = Wt::WLink(Wt::LinkType::InternalPath, "/");
  linkNewGame_.setTarget(Wt::LinkTarget::ThisWindow);
  anchorNewGame_ = root()->addWidget(std::make_unique<Wt::WAnchor>(linkNewGame_, "New Game"));
  anchorNewGame_->setStyleClass("button");
  anchorNewGame_->disable();
  anchorNewGame_->hide(); // hide the new game button until the game is over

  // Build a timer to auto populate the wordle link and check validy as it is typed
  auto timer = root()->addChild(std::make_unique<Wt::WTimer>());
  timer->setInterval(std::chrono::seconds(1));
  timer->timeout().connect(this, &Wordle::validityCheck); // every second call validityCheck()
  timer->start();

  // On any text change call validityCheck()
  nameEdit_->changed().connect(std::bind(&Wordle::validityCheck, this));

  // On button press, call submitGuess()
  buttonSubmit_->clicked().connect(this, &Wordle::submitGuess);

  // On enter press, call enterGuess()
  nameEdit_->enterPressed().connect(std::bind(&Wordle::enterGuess, this));

}

/**
 * Function: Wordle destructor
 * Description: Destroy a Wordle application
 * Parameters: None
 * Return Value: None
*/
Wordle::~Wordle()
{
  allWords_.clear();
  usedWordIdxs_.clear();
  usedWords_.clear();
  curAnswerChars_.clear();
  curGuessChars_.clear();

  curAnswer_.clear();
  curGuess_.clear();

  // WT components cleaned up automatically, when parent widget is deleted
}



/**
 * Function name: isValidWord
 * Description: Determines whther the current word is a valid 5 letter word
 * Parameters: None
 * Return Value: true if the current guess is valid, false otherwise
*/
bool Wordle::isValidWord() {
  
  for(auto i = 0; i < allWords_.size(); ++i ) {
    std::string curWord = boost::to_upper_copy<std::string>(allWords_.at(i));
    std::wstring curWideWord(curWord.begin(), curWord.end()); // make comparable strings
    if (curGuess_ == curWideWord )  {
      return true;
    } 
  }

  return false;
}

/**
 * Function name: isWordNew
 * Description: Determines whther the current word is new or repeated
 * Parameters: None
 * Return Value: true if the current guess is a new word, false otherwise
*/
bool Wordle::isWordNew() { 
  if (usedWords_.count(curGuess_) == 0) {
    return true;
  }
  return false;
}

/**
 * Function name: isWin
 * Description: Determines whether the current guess is a win
 * Parameters: None
 * Return Value: true if the current guess is a win, false otherwise
*/
bool Wordle::isWin() {
  std::wstring curAnswer(curAnswer_.begin(), curAnswer_.end());
  if ( curGuess_ == curAnswer ) {
    return true;
  }
    return false;
}

/**
 * Function name: setWin
 * Description: Sets the game into a win state
 * Parameters: std::wstring curAnswer
 * Return Value: none
*/
void Wordle::setWin(const std::wstring curAnswer) {
  if ( curGuess_ == curAnswer ) {
    for(auto i = 0; i < WORD_SIZE; ++i) {
      curState_[guessNum_][i]->setStyleClass("correctLetterSpot");
    }
    nameEdit_->hide();
    buttonSubmit_->hide();
    gameMessage_->setText("You Win!");
    buttonSubmit_->disable();
    nameEdit_->disable();
    anchorNewGame_->enable();
    anchorNewGame_->show();
  }
}

/**
 * Function name: validityCheck
 * Description: Checks the validity of the current guess, updates the board and game state on both 
 *              editbox changes and every second
 * Parameters: None
 * Return Value: none
*/
void Wordle::validityCheck()
{
  /*
   * Update the text, using text input into the nameEdit_ field.
   */

  if (isWin()) {
    return; // do nothing if the game is already won
  }


  if (guessNum_ == NUM_GUESSES) { // Out of guesses so set the game over state and messages
    nameEdit_->hide();
    buttonSubmit_->hide();
    answerWordMessage_->setText("The answer was: " + curAnswer_);
    answerWordMessage_->show();
    gameMessage_->setText("You Lose!");
    buttonSubmit_->disable();
    nameEdit_->disable();
    anchorNewGame_->enable();
    anchorNewGame_->show();
    return;
  }


  Wt::WString curGuess = nameEdit_->text();
  std::wstring curGuessString = curGuess.value();
  curGuess_ = curGuessString;

  int curGuessLength = curGuessString.length();

  if(curGuessLength != WORD_SIZE) {
    buttonSubmit_->disable();
  } 
  else {
    if (isValidWord() && isWordNew()) {
      buttonSubmit_->enable();  // Typed input is large enough, is a valid word, and is a new word so enable the submit button
    } 
    else {
      buttonSubmit_->disable(); // Typed input is large enough, either not valid word or not new word so disable the submit button
    }
  }

  // Color the current guess letters in the board
  for (auto i = 0; i < curGuessLength; ++i) {
    std::wstring curChar = curGuessString.substr(i,1);
    curState_[guessNum_][i]->setText(curChar);
    curState_[guessNum_][i]->setStyleClass("");
  }
  for (auto i = curGuessLength; i < WORD_SIZE; ++i) {
    curState_[guessNum_][i]->setStyleClass("guess");
    curState_[guessNum_][i]->setText("|_|");
  }
  if (curGuessLength < 5) {
    curState_[guessNum_][curGuessLength]->setStyleClass("currentLetter");
  }

  // Update the game message for the current guess number
  gameMessage_->setText("Guess Number: " + std::to_string(guessNum_ + 1) );
}


/**
 * Function name: enterGuess
 * Description: submits the current edit box entry as a guess. Determines whether to call submitGuess()
 * Parameters: None
 * Return Value: none
*/
void Wordle::enterGuess(){
  if (isValidWord() && isWordNew() ) {
    submitGuess();
  }
}

/**
 * Function name: submitGuess
 * Description: submits the current edit box entry as a guess. Checks validity, whether it is a win
 * Parameters: None
 * Return Value: none
*/
void Wordle::submitGuess()
{
  
  usedWords_.insert(curGuess_);

  validityCheck();

  if (isWin()) {
    std::wstring curAnswer(curAnswer_.begin(), curAnswer_.end());
    setWin(curAnswer);
    return;
  }

  buttonSubmit_->disable();

  for (auto i = 0; i < WORD_SIZE; ++i) {
    curState_[guessNum_][i]->setStyleClass(""); // reset the style class for the current guess
    
  }
  correctLetterCheck(); // check the current guess for correct letters in the correct and wrong spots

  if(guessNum_ < NUM_GUESSES) {
    guessNum_++;
    nameEdit_->setText("");
    nameEdit_->setFocus();
  }
}

/**
 * Function name: correctLetterCheck
 * Description: Checks for correct letters in the correct and wrong spots and updates state accordingly
 * Parameters: None
 * Return Value: none
*/
void Wordle::correctLetterCheck() {

  std::string curGuess (curGuess_.begin(), curGuess_.end()); // make comparable strings

  for(char& c : curGuess) {
    curGuessChars_.insert(c); // insert the current guess characters into a multiset
  }

  std::multiset<char> curCorrectSpotLetters; // multiset of correct letters in the correct spots

  for(auto i = 0; i < WORD_SIZE; ++i) {
    if (curGuess_[i] == curAnswer_[i]) {
      curCorrectSpotLetters.insert(curGuess_[i]);
    }
  }
  
  // Find the intersection of the current guess characters and the current answer characters
  std::multiset<char> intersect;
  set_intersection(curAnswerChars_.begin(), curAnswerChars_.end(), 
                  curGuessChars_.begin(), curGuessChars_.end(),
                  std::inserter(intersect, intersect.begin()));

  intersect.size();

  std::multiset<char> correctLettersWrongSpots;

  for(auto i = 0; i < WORD_SIZE; ++i) {

    // Determine if it is the correct letter in correct spot
    if (curGuess_[i] == curAnswer_[i]) {
      curState_[guessNum_][i]->setStyleClass("correctLetterSpot");
      
      auto it = intersect.find(curGuess_[i]);
      // Check if the element is found
      if (it != intersect.end()) {
      // Erase the element
        intersect.erase(it);
      }
      
      continue;
    }

    // Determine if it is the correct letter in the wrong spot
    std::set_difference(intersect.begin(), intersect.end(), 
                  curCorrectSpotLetters.begin(), curCorrectSpotLetters.end(),
                  std::inserter(correctLettersWrongSpots, correctLettersWrongSpots.begin()));

    for (auto j = 0; j < correctLettersWrongSpots.size(); ++j) {
        if( correctLettersWrongSpots.count(curGuess_[i]) > 0) {
          auto it = correctLettersWrongSpots.find(curGuess_[i]);
          // Step 2: Check if the element is found
          if (it != correctLettersWrongSpots.end()) {
            // Step 3: Erase the element
            correctLettersWrongSpots.erase(it);
          }

          // intersect.erase(curGuess_[i]);
          curState_[guessNum_][i]->setStyleClass("correctLetterWrongSpot");
        }
    }
  }
}
