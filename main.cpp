/**
 * File Name: main.cpp
 * Description: Implement the main function entrypoint.
 * Author: Amy Uccello
 * Student Number: 251236036
 * Assignment Number: 1
 * Class: CS3307
 * Date Created: September 30, 2023
*/

#include "main.h"


/**
 * Function name: main
 * Description: The main function entrypoint.
 * Parameters: argc - the number of arguments
 *             argv - the arguments
 * Return Value: 0 on success, 1 on failure
*/
int main(int argc, char **argv)
{
  
  // Run the Wt Wordle application
  return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
    
    return std::make_unique<Wordle>(env); // create a new instance of the Wordle application
    
  });
}
