#ifndef WINDOW_H
#define WINDOW_H

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace Tearsplash
{

	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

	class Window
	{
	public:
		Window();
		~Window();

		int createWindow(std::string windowName, int windowWidth, int windowHeight, unsigned int currentFlags);
		int getScreenWidth() { return mWindowWidth; }
		int getScreenHeight() { return mWindowHeight; }
		void swapBuffer();
    SDL_Window* getSDLWindow() {
      return mSDLWindow;
    }

    SDL_GLContext getGLContext() {
      return mGLContext;
    }



	private:
		SDL_Window* mSDLWindow;
    SDL_GLContext mGLContext;
		int mWindowWidth;
		int mWindowHeight;

	};

}

#endif // !WINDOW_H
