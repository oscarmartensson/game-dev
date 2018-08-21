// MainGame.h

#ifndef MAINGAME_H
#define MAINGAME_H

// ------------ Includes ------------

#ifdef  _WIN32
// Include for windows if target is windows
#include <Windows.h>
#endif //  _WIN32

#include <SDL/SDL.h>	// Used for window and input
#include <GL/glew.h>	// Used for OpenGL
#include <iostream>

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	void initSystems();

private:
	SDL_Window* mpWindow;
	int			mWindowWidth;
	int			mWindowHeight;
};

#endif // !MAINGAME_H


