// MainGame.h

#ifndef MAINGAME_H
#define MAINGAME_H

// ------------ Includes ------------
#if defined( _WIN32) || defined(_WIN64)
// Include for windows if target is windows
#include <Windows.h>
#endif //  _WIN32

// STL includes
#include <vector>
#include <iostream>

#include <SDL/SDL.h>	// Used for window and input

#include <GL/glew.h>	// Used for OpenGL
#include <Box2D/Box2D.h>

// Tearsplash engine
#include <Tearsplash/Tearsplash.h>
#include <Tearsplash/ShaderProgram.h>
#include <Tearsplash/GLTexture.h>
#include <Tearsplash/Window.h>
#include <Tearsplash/Camera2D.h>
#include <Tearsplash/Spritebatch.h>
#include <Tearsplash/Spritefont.h>
#include <Tearsplash/InputManager.h>
#include <Tearsplash/Timing.h>
#include <Tearsplash/AudioEngine.h>
#include <Tearsplash/Box.h>

#include "Projectile.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();
	void run();

private:
	// Functions
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void render();
	void printFPS();
    void createPhysicsObjects();
    void updatePhysics();

	// Member variables
    GameState						 mCurrentGameState;
	Tearsplash::Window				 mWindow;
    Tearsplash::ShaderProgram		 mColorShaders;
    Tearsplash::Camera2D             mCamera;
    Tearsplash::Spritebatch          mSpritebatch;
    Tearsplash::InputManager         mInputManager;
    Tearsplash::FPSLimiter           mFPSLimiter;
	Tearsplash::AudioEngine          mAudioEngine;
    Tearsplash::Spritefont           mHUDText;
    std::vector<Projectile>          mBullets;


    float                            mFPS;
    float                            mMaxFPS;
	int                              mWindowWidth;
	int                              mWindowHeight;
    std::unique_ptr<b2World>         mPhysicsWorld;
    b2Vec2                           mGravity;
    std::vector<Tearsplash::Box>     mPhysicsBoxes;
};

#endif // !MAINGAME_H


