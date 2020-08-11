/**********************************************************************/
// File:	MainGame.cpp
// Summary:	This is the class that runs the main and most high level
//			portion of the game. Here, things are initialized and run.
//
// Version: Version 1.0 - 2018-08-16
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-16 File created
//          2019-03-24 Added timing class and input manager
/**********************************************************************/

// Includes -------------------------
#include <string>

#include <Tearsplash/Errors.h>
#include <Tearsplash/ImageLoader.h>
#include <Tearsplash/ResourceManager.h>

#include "MainGame.h"


// ----------------------------------
// Default constructor
MainGame::MainGame() : 
	mCurrentGameState(GameState::PLAY), 
	mWindowWidth(1280), mWindowHeight(720),  
	mMaxFPS(60.0f),
    mFPS(0.0f)
{
	// Initialize member variables through MIL
}

// ----------------------------------
// Default destructor
MainGame::~MainGame()
{
    mAudioEngine.destroy();
}

// ----------------------------------
// Runs the game
void MainGame::run()
{
	initSystems();

	gameLoop();
}

// ----------------------------------
// Initializes vital game engine systems such as memory allocation, SDL (for input and creating window),
// etc.
void MainGame::initSystems()
{
	Tearsplash::init();
    mAudioEngine.init();

    mFPSLimiter.init(mMaxFPS);

	mWindow.createWindow("Tearsplash", mWindowWidth, mWindowHeight, SDL_WINDOW_FULLSCREEN_DESKTOP);
    mCamera.init(mWindowWidth, mWindowHeight);

	initShaders();
    mSpritebatch.init();

    mHUDText.init("fonts/28_Days_Later.ttf");
}

// ----------------------------------
// Main game loop.
void MainGame::gameLoop()
{
	// Keep looping while player hasn't pressed exit
	while (mCurrentGameState != GameState::EXIT)
	{
        mFPSLimiter.begin();

		float startTicks = static_cast<float>(SDL_GetTicks());
		processInput();

        mCamera.update();

        // Update all bullets
        for (size_t i = 0; i < mBullets.size();)
        {
            if (mBullets[i].update() == true)
            {
                mBullets[i] = mBullets.back();
                mBullets.pop_back();
            }
            else
            {
                i++;
            }
        }

		render();

        mFPS = mFPSLimiter.end();

        printFPS();
	}

	return;
}

// ----------------------------------
// Processes user input.
void MainGame::processInput()
{
    const float SCALE_SPEED = 0.1f;
    const float CAMERA_SPEED = 5.0f;

    mInputManager.update();
	SDL_Event userInput;

	while (SDL_PollEvent(&userInput) == 1)
	{
		// There is a user input present
		switch (userInput.type)
		{
			case SDL_QUIT:
				mCurrentGameState = GameState::EXIT;
				break;

			case SDL_MOUSEMOTION:
                mInputManager.setMouseCoords(static_cast<float>(userInput.motion.x), static_cast<float>(userInput.motion.y));
                break;

            case SDL_KEYDOWN:
                mInputManager.pressKey(userInput.key.keysym.sym);
                break;

            case SDL_KEYUP:
                mInputManager.releaseKey(userInput.key.keysym.sym);
                break;

            case SDL_MOUSEBUTTONDOWN:
                mInputManager.pressKey(userInput.button.button);
                break;

            case SDL_MOUSEBUTTONUP:
                mInputManager.releaseKey(userInput.button.button);
                break;

			default:
				// Do nothing
				break;
		}
	}

    // Check for key pressed in input manager and add action

    if (mInputManager.isKeyPressed(SDLK_w))
    {
        // NOTE! Camera is moving down, scene moving up
        mCamera.setPosition(mCamera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
    }

    if (mInputManager.isKeyPressed(SDLK_s))
    {
        // NOTE! Camera is moving up, scene moving down
        mCamera.setPosition(mCamera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
    }

    if (mInputManager.isKeyPressed(SDLK_a))
    {
        // NOTE! Camera is moving right, scene moving left
        mCamera.setPosition(mCamera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
    }

    if (mInputManager.isKeyPressed(SDLK_d))
    {
        // NOTE! Camera is moving left, scene moving right
        mCamera.setPosition(mCamera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
    }

    if (mInputManager.isKeyPressed(SDLK_q))
    {
        mCamera.setScale(mCamera.getScale() - SCALE_SPEED);
    }

    if (mInputManager.isKeyPressed(SDLK_e))
    {
        mCamera.setScale(mCamera.getScale() + SCALE_SPEED);
    }

    if (mInputManager.isKeyPressed(SDLK_f))
    {
        glm::vec2 mouseCoords = mInputManager.getMouseCoords();
        mouseCoords = mCamera.convertScreen2World(mouseCoords);
        //std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;

        glm::vec2 playerPosition(0.0f);
        glm::vec2 direction = mouseCoords - playerPosition;
        direction = glm::normalize(direction);

        // This is really ugly, should not try to load the sound effect every time a bullet is fired.
        mBullets.emplace_back(playerPosition, direction, 10.0f, 1000, mAudioEngine.loadSoundEffect("sound/shots/pistol.wav"));
        mBullets.back().playSoundFX();
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

  
    // Set uniforms
    GLint textureLocation = mColorShaders.getUniformLocation("texSampler");
    glUniform1i(textureLocation, 0);


    GLint pLocation = mColorShaders.getUniformLocation("P");
    glm::mat4 cameraMatrix = mCamera.getCameraMatrix();
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	
    // Start filling sprite batches
    mSpritebatch.begin(Tearsplash::GlyphSortType::TEXTURE);

    glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    static Tearsplash::GLTexture texture = Tearsplash::ResourceManager::getTexture("textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
    Tearsplash::ColorRGBA8 color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;

    // Draw the player sprite.
    mSpritebatch.draw(pos, uv, texture.id, 0, color);

    for (size_t i = 0; i < mBullets.size(); i++)
    {
        // Render the bullets to the sprite batch.
        if (mCamera.isInView(mBullets[i].getPosition(), mBullets[i].getAABB())) {
            mBullets[i].draw(mSpritebatch);
        }
    }

    // Stop filling sprite batches
    mSpritebatch.end();

    // Render sprite batches
    mSpritebatch.renderBatch();

	// Stop using shader program
	mColorShaders.dontuse();

    // Render Text
    mHUDText.drawText("This is a test", glm::vec4(100.0f, 100.0f, 0.0f, 0.0f), glm::ortho(0.0f, static_cast<float>(mWindowWidth), 0.0f, static_cast<float>(mWindowHeight)), glm::vec3(1.0f, 1.0f, 1.0f), 10.0f);
    mHUDText.render();

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
// Prints fps each 10 frames
void MainGame::printFPS()
{
	static int frameCounter = 0;

	frameCounter++;

	// Print each 10th frame
	if (frameCounter == 100)
	{
		std::cout << "fps: " << mFPS << std::endl;
		frameCounter = 0;
	}
}