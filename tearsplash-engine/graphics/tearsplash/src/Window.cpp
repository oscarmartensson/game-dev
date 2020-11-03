/**********************************************************************/
// File:	Window.cpp
// Summary:	This is a class which constructs a window for the user to 
//			display favourite graphics in.
//
// Version: Version 1.0 - 2019-02-26
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2019-02-26 File created
/**********************************************************************/

#include "Tearsplash/Errors.h"
#include "Tearsplash/Window.h"

using namespace Tearsplash;

// Constructor
Window::Window() {}

// Destructor
Window::~Window() {}

int Window::createWindow(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
{
	// Update window flags if needed
	Uint32 flags = SDL_WINDOW_OPENGL;
	if (currentFlags & INVISIBLE)
	{
    flags |= SDL_WINDOW_HIDDEN;
	}
	else if (currentFlags & FULLSCREEN)
	{
    flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	else if (currentFlags & BORDERLESS)
	{
    flags |= SDL_WINDOW_BORDERLESS;
	}

  // Set flags for context. Create core context profile.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Create window
	mSDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
	if (mSDLWindow == nullptr)
	{
		fatalError("SDL window could not be created");
	}

	// Set member variables
	mWindowHeight = screenHeight;
	mWindowWidth = screenWidth;

	// Init GL
	mGLContext = SDL_GL_CreateContext(mSDLWindow);
	if (mGLContext == nullptr)
	{
		fatalError("SDL_GL context could not be created");
	}
  SDL_GL_MakeCurrent(mSDLWindow, mGLContext);

	// Init glew
#ifdef DEBUG
	glewExperimental = true;						// To counter some errors
#endif
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		fatalError("GLEW could not be initialized");
	}

	// Print OpenGL version of system
	std::printf("--- OpenGL version %s ---\n\n", glGetString(GL_VERSION));

	// Clear bg color to blue
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Disable vertical sync
	SDL_GL_SetSwapInterval(0);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Window::swapBuffer()
{
	// Swap render buffer
	SDL_GL_SwapWindow(mSDLWindow);
}