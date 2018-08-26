/**********************************************************************/
// File:	main.cpp
// Summary:	TThis is the main function for the Tearsplash game engine.
//			Since there is such as vast amount of successful engines with
//			names similar to ice and drop, I figured I'd give it a shot.
//
// Version: Version 1.0 - 2018-08-16
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-16 File created
/**********************************************************************/

// Includes -------------------------
#include "MainGame.h"

// ----------------------------------
// Main function
int main(int argc, char** argv) // Argc and argv are needed since the "real" main function reside in the SDL library
{
	// Create the main game object and run
	MainGame mainGame;
	mainGame.run();

	return 0;
}