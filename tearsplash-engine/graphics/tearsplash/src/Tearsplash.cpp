/**********************************************************************/
// File:	Tearsplash.cpp
// Summary:	This file contains the init function for the Tearsplash engine
//
// Version: Version 1.0 - 2019-03-12
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2019-03-12 File created
/**********************************************************************/

#include <SDL/SDL.h>	// Used for window and input
#include <GL/glew.h>	// Used for OpenGL

#include <Tearsplash/Tearsplash.h>

//using namespace Tearsplash;

int Tearsplash::init()
{
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Tell OpenGL to use double buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return 0;
}