// MainGame.h

#ifndef MAINGAME_H
#define MAINGAME_H

// ------------ Includes ------------
#ifdef  _WIN32
// Include for windows if target is windows
#include <Windows.h>
#endif //  _WIN32

#include <vector>
#include <SDL/SDL.h>	// Used for window and input
#include <GL/glew.h>	// Used for OpenGL
#include <iostream>
#include "Sprite.h"
#include "ShaderProgram.h"
#include "GLTexture.h"

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
	void calcFPS();
	void printFPS();

	// Member variables
	SDL_Window*			 mpWindow;
	GameState			 mCurrentGameState;
	int					 mWindowWidth;
	int					 mWindowHeight;
	float				 mTime;
	std::vector<Sprite*> mSprites;
	ShaderProgram		 mColorShaders;
	float				 mFrameTime;
	float				 mFPS;
	float				 mMaxFPS;
};

#endif // !MAINGAME_H


