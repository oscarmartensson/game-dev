/**********************************************************************/
// File:	Errors.cpp
// Summary:	This is a utility file which handles errors that come up 
//			during runtime.
//
// Version: Version 1.1 - 2018-09-08
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-23 File created
//			2018-09-08 Updated filed with soft error
/**********************************************************************/

// Includes -------------------------
#include <SDL/SDL.h>
#include <iostream>
#include <cstdlib>
#include "Errors.h"

// ----------------------------------
// Takes an error message for fatal error, displays it,
// and exits the engine and system.
void fatalError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to exit..." << std::endl;
	int tmp;
	std::cin >> tmp;

	// Shut down SDL and exit system
	SDL_QUIT;
	exit(1337);
}

// ----------------------------------
// Takes an error message for a soft
// non-critical error and displays it
void softError(std::string errorString)
{
	std::cout << errorString << std::endl;
}