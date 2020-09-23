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
#include <Tearsplash/ParticleEngine2D.h>

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
    void updatePhysics(const float timeStep);
    void initParticleSystem();

	// Member variables
    GameState						 mCurrentGameState;
	Tearsplash::Window				 mWindow;
    Tearsplash::ShaderProgram		 mColorShaders;
    Tearsplash::Camera2D             mCamera;
    Tearsplash::Spritebatch          mSpritebatch;
    Tearsplash::Spritebatch          mSpritebatchParticles;
    Tearsplash::InputManager         mInputManager;
    Tearsplash::FPSLimiter           mFPSLimiter;
	Tearsplash::AudioEngine          mAudioEngine;
    Tearsplash::Spritefont           mHUDText;
    Tearsplash::ParticleEngine2D     mParticleEngine;
    std::vector<Projectile>          mBullets;
    glm::vec2                        mPlayerPosition;
    glm::vec2                        mPlayerDirection;
    glm::vec2                        mParticleVelocity;
    Tearsplash::ColorRGBA8           mParticleColor;
    Tearsplash::GLTexture            mParticleTexture;

    Tearsplash::ParticleBatch2D      mParticleBatch2D;



    float                            mFPS;
    float                            mMaxFPS;
	int                              mWindowWidth;
	int                              mWindowHeight;
    std::unique_ptr<b2World>         mPhysicsWorld;
    b2Vec2                           mGravity;
    std::vector<Tearsplash::Box>     mPhysicsBoxes;
    std::vector<Tearsplash::GLTexture> mTextures;
};

#endif // !MAINGAME_H


