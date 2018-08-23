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

// Includes -------------------------
#include <string>
#include "MainGame.h"
#include "Errors.h"

// ----------------------------------
// Default constructor
MainGame::MainGame() : mpWindow(nullptr), mWindowWidth(1280), mWindowHeight(720), mCurrentGameState(GameState::PLAY)
{
	// Initialize member variables through MIL
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

	mSprite.init(-1.0f, -1.0f, 1.0f, 1.0f);
	gameLoop();
}

// ----------------------------------
// Initializes vital game engine systems such as memory allocation, SDL (for input and creating window),
// etc.
void MainGame::initSystems()
{
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Create window
	mpWindow = SDL_CreateWindow("Tearsplash engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowWidth, mWindowHeight, SDL_WINDOW_OPENGL);
	if (mpWindow == nullptr)
	{
		fatalError("SDL window could not be created");
	}

	// Init GL
	SDL_GLContext glContext = SDL_GL_CreateContext(mpWindow);
	if (glContext == nullptr)
	{
		fatalError("SDL_GL context could not be created");
	}

	// Init glew
#ifdef DEBUG
	glewExperimental = true;	// To counter some errors
#endif
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		fatalError("GLEW could not be initialized");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// Tell OpenGL to use double buffer
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);			// Clear bg color to blue

	initShaders();
}

// ----------------------------------
// Main game loop.
void MainGame::gameLoop()
{
	while (mCurrentGameState != GameState::EXIT)
	{
		// Keep looping while player hasn't pressed exit
		processInput();
		render();
	}
	return;
}

// ----------------------------------
// Processes user input.
void MainGame::processInput()
{
	SDL_Event userInput;

	while (SDL_PollEvent(&userInput) == (int)1)
	{
		// There is a user input present
		switch (userInput.type)
		{
			case SDL_QUIT:
				mCurrentGameState = GameState::EXIT;
				break;

			case SDL_MOUSEMOTION:
				std::cout << userInput.motion.x << " " << userInput.motion.y << std::endl;

			default:
				// Do nothing
				break;
		}
	}
}

// ----------------------------------
// Rendering main function
void MainGame::render()
{
	// Prepare for rendering
	glClearDepth(1.0f);									// Clear depth to 1
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear color buffer and depth buffer between draws

	mColorShaders.use();

	mSprite.draw();

	mColorShaders.dontuse();

	SDL_GL_SwapWindow(mpWindow);						// Swap render buffer
}

// ----------------------------------
// Initializes shaders
void MainGame::initShaders()
{
	mColorShaders.compileShaders("shaders/colorShading.vert", "shaders/colorShading.frag");

	mColorShaders.addAttribute("vertexPosition");

	mColorShaders.linkShaders();
}