#ifndef WINDOW_H
#define WINDOW_H

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

namespace Tearsplash
{

	enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4, RESIZABLE = 0x8};

	class Window
	{
	public:
		Window();
		~Window();

		int createWindow(std::string windowName, int windowWidth, int windowHeight, unsigned int currentFlags);
		int getScreenWidth() const { return mWindowWidth; }
		int getScreenHeight() const { return mWindowHeight; }
		void swapBuffer();
    SDL_Window* getSDLWindow() {
      return mSDLWindow;
    }

    SDL_GLContext getGLContext() const {
      return mGLContext;
    }

    const char* getGLSLVersion() const {
      return mGLSLVersion;
    }

	private:
		SDL_Window* mSDLWindow;
    SDL_GLContext mGLContext;
		int mWindowWidth;
		int mWindowHeight;
    const char* mGLSLVersion = "#version 460";
	};

}

#endif // !WINDOW_H
