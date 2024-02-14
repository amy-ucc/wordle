/**
 * File Name: Wordle.h
 * Description: This is the header file for the Wordle class.
 * Author: Amy Uccello
 * Student Number: 251236036
 * Assignment Number: 1
 * Class: CS3307
 * Date Created: September 30, 2023
*/


#ifndef _WORDLE_
#define _WORDLE_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <wctype.h>
#include <boost/algorithm/string.hpp>
#include <boost/regex/pending/unicode_iterator.hpp>
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WTimer.h>
#include <Wt/WTable.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>


/*
 * Wordle Application Class
 */
class Wordle : public Wt::WApplication
{
  
public:
  Wordle(const Wt::WEnvironment& env);
  ~Wordle();

private:
  
  const int WORD_SIZE = 5;
  const int NUM_GUESSES = 6;
  int guessNum_; // Tracks the guess number
  
  std::vector<std::string> allWords_; // Vector of all the 5 letter words
  std::set<int> usedWordIdxs_; // Keeps track of which words have already been used in a session so there is no repetition
  std::set<std::wstring> usedWords_; // Keeps track of which words have already been used in a session so there is no repetition
  std::string curAnswer_; // The current answer word as a string
  std::multiset<char> curAnswerChars_; // The current answer as a multiset of characters
  std::wstring curGuess_; // The current answer guess as a wstring
  std::multiset<char> curGuessChars_; // The current guess as a multiset of characters
  
  // The board represented as a table and its elements
  Wt::WTable *curBoard_;
  Wt::WText *curState_[6][5];

  Wt::WLineEdit *nameEdit_; // The edit box where the user can enter a guess
  Wt::WText     *gameMessage_; // The text box where game messages are displayed
  Wt::WPushButton *buttonSubmit_; // The button to submit a guess entier in the nameEdit_ box
  Wt::WText *answerWordMessage_; // The text box where the answer word will be displayed
  
  // Wt::WPushButton *buttonReload_; // The button to reload the page and start a new game
  Wt::WAnchor *anchorNewGame_;  // The anchor to reload the page and start a new game
  Wt::WLink linkNewGame_; // The link included in the anchorNewGame_ anchor

  bool isValidWord(); // Checks if the word entered in nameEdit_ is valid. buttonSubmit_ is enabled / disabled based on this
  bool isWordNew(); // Checks if the word entered in nameEdit_ a new or repeated word. buttonSubmit_ is enabled / disabled based on this
  bool isWin(); // Checks if the word entered in nameEdit_ that has been submitted is the correct answer. Also sets the winning state.
  void setWin(const std::wstring curAnswer); // Sets the winning state

  void validityCheck(); // Checks the validity of the word entered in nameEdit_, enables / disables buttonSubmit_, colors the board
  void enterGuess(); // Responds to enter and determines if submitGuess() should be called
  void submitGuess(); // Responds to the buttonSubmit_ button to determine if the game is won and if not what state it should be in

  void correctLetterCheck(); // Checks for the correct letters in the correct and wrong spots and updates state accordingly
  
};

#endif // _WORDLE_
