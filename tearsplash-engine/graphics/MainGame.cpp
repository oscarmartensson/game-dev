/**********************************************************************/
// File:	MainGame.cpp
// Summary:	This is the class that runs the main and most high level
//			portion of the game. Here, things are initialized and run.
//
// Version: Version 1.0 - 2018-08-16
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-16 File created
/**********************************************************************/

#include "MainGame.h"



// ----------------------------------
// Default constructor
MainGame::MainGame()
{
	mpWindow		= NULL;
	mWindowWidth	= 1280;
	mWindowHeight	= 720;
}

// ----------------------------------
// Default destructor
MainGame::~MainGame()
{
	// Do nothing
}

// ----------------------------------
// Runs the game
void MainGame::run()
{
	initSystems();
}

// ----------------------------------
// Initializes vital game engine systems such as memory allocation, SDL (for input and creating window),
// etc.
void MainGame::initSystems()
{
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	mpWindow = SDL_CreateWindow("Tearsplash engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, SDL_WINDOW_OPENGL);
	if (mpWindow == NULL)
	{
		std::cout << "Failed to create SDL window. Shutting down.... \n";
		return;
	}
}