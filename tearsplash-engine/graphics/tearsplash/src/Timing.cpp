/**********************************************************************/
// File:	Timing.cpp
// Summary:	Handles various timing such as fps, limits etc.
//
// Version: Version 1.0 - 2019-03-24
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2019-03-24 File created
/**********************************************************************/

#include "Tearsplash/Timing.h"
#include "Tearsplash/Errors.h"

#include <SDL/SDL.h>

using namespace Tearsplash;


FPSLimiter::FPSLimiter() {}
FPSLimiter::~FPSLimiter() {}

void FPSLimiter::init(const float maxFPS)
{
    setMaxFPS(maxFPS);
}

void FPSLimiter::begin() 
{
    mStartTicks = SDL_GetTicks();
}

// Returns the current fps
float FPSLimiter::end()
{
    calcFPS();

    float frameTicks = SDL_GetTicks() - mStartTicks;
    float maxFrameTicks = 1000.0f / mMaxFPS;

    // Check if we need to wait
    if (maxFrameTicks > frameTicks)
    {
        SDL_Delay(maxFrameTicks - frameTicks);
    }

    return mFPS;
}

// ----------------------------------
// Calculates the engine FPS averaged over NUM_SAMPLES of frames
void FPSLimiter::calcFPS()
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
        Tearsplash::softError("FPS counter division by zero.");
    }

    // Update ticks
    previousTicks = currentTicks;
}