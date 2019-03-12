/**********************************************************************/
// File:	MainGame.cpp
// Summary:	This is the class that runs the main and most high level
//			portion of the game. Here, things are initialized and run.
//
// Version: Version 1.0 - 2018-08-16
// Author:	Oscar M�rtensson
// -------------------------------------------
// Log:	    2018-08-16 File created
/**********************************************************************/

// Includes -------------------------
#include <string>
#include <Tearsplash/Errors.h>
#include <Tearsplash/ImageLoader.h>

#include "MainGame.h"


// ----------------------------------
// Default constructor
MainGame::MainGame() : 
	mCurrentGameState(GameState::PLAY), 
	mWindowWidth(1280), mWindowHeight(720),  
	mTime(0.0f), 
	mMaxFPS(60.0f)
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

	mSprites.push_back(new Sprite());
	mSprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	mSprites.push_back(new Sprite());
	mSprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");

	gameLoop();
}

// ----------------------------------
// Initializes vital game engine systems such as memory allocation, SDL (for input and creating window),
// etc.
void MainGame::initSystems()
{
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Tell OpenGL to use double buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	

	mWindow.createWindow("Tearsplash", mWindowWidth, mWindowHeight, SDL_WINDOW_FULLSCREEN_DESKTOP);

	initShaders();
}

// ----------------------------------
// Main game loop.
void MainGame::gameLoop()
{
	// Keep looping while player hasn't pressed exit
	while (mCurrentGameState != GameState::EXIT)
	{
		float startTicks = static_cast<float>(SDL_GetTicks());
		processInput();
		mTime += .001;
		render();
		calcFPS();
		printFPS();

		float frameTicks = SDL_GetTicks() - startTicks;
		float maxFrameTicks = 1000.0f / mMaxFPS;

		// Check if we need to wait
		if (maxFrameTicks > frameTicks)
		{
			SDL_Delay(maxFrameTicks - frameTicks);
		}
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

	// Use shader program and set first texture (0)
	mColorShaders.use();
	glActiveTexture(GL_TEXTURE0);
;
	GLint textureLocation = mColorShaders.getUniformLocation("texSampler");
	glUniform1i(textureLocation, 0);

	// Set uniforms
	GLint location = mColorShaders.getUniformLocation("time");
	glUniform1f(location, mTime);

	// Draw sprites
	for (int i = 0; i < mSprites.size(); i++)
	{
		mSprites[i]->draw();
	}
	
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Stop using shader program
	mColorShaders.dontuse();

	mWindow.swapBuffer();
}

// ----------------------------------
// Initializes shaders
void MainGame::initShaders()
{
	mColorShaders.compileShaders("shaders/colorShading.vert", "shaders/colorShading.frag");

	// Setup attribute pointers
	mColorShaders.addAttribute("vertexPosition");
	mColorShaders.addAttribute("vertexColor");
	mColorShaders.addAttribute("vertexUV");

	mColorShaders.linkShaders();
}

// ----------------------------------
// Calculates the engine FPS averaged over NUM_SAMPLES of frames
void MainGame::calcFPS()
{
	static const int	NUM_SAMPLES = 10;
	static int			currentFrame = 0;
	static float		frameTimes[NUM_SAMPLES];
	static float		previousTicks = SDL_GetTicks();
	float				currentTicks = SDL_GetTicks();
	float				frameTimeAverages = 0.0f;
	int count;

	mFrameTime = currentTicks - previousTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = mFrameTime;

	currentFrame++;
	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else
	{
		count = NUM_SAMPLES;
	}

	// Calculate the average frame time
	frameTimeAverages = 0.0f;
	for (int i = 0; i < count; i++)
	{
		frameTimeAverages += frameTimes[i];
	}

	// Check for divison-by-zero
	if (count > 0)
	{
		frameTimeAverages /= count;
	}

	// Check for divison-by-zero
	if (frameTimeAverages > 0)
	{
		// Convert from [f / ms] -> [f / s]
		mFPS = 1000.0f / frameTimeAverages;
	}
	else
	{
		softError("FPS counter division by zero.");
	}
	
	// Update ticks
	previousTicks = currentTicks;
}

// ----------------------------------
// Prints fps each 10 frames
void MainGame::printFPS()
{
	static int frameCounter = 0;

	frameCounter++;

	// Print each 10th frame
	if (frameCounter == 10)
	{
		std::cout << "fps: " << mFPS << std::endl;
		frameCounter = 0;
	}
}