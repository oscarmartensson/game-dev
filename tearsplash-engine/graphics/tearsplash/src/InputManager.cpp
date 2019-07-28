/**********************************************************************/
// File:	InputManager.cpp
// Summary:	Handles user input and adds various quality-of-life improvements
//          to input and controller feel that wouldn't be possible without 
//          an input handler class.
//
// Version: Version 1.0 - 2019-03-24
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2019-03-24 File created
/**********************************************************************/

#include "Tearsplash/InputManager.h"

using namespace Tearsplash;

InputManager::InputManager() : mMouseCoords(0.0f) {}

InputManager::~InputManager() {}

void InputManager::update()
{
    for (auto it = mKeyMap.begin(); it != mKeyMap.end(); it++)
    {
        // Copies current keyMap to previous
        mPrevKeyMap[it->first] = it->second;
    }
}

void InputManager::pressKey(const unsigned int keyID)
{
    // First checks if keyID exists, if not, creates new entry
    mKeyMap[keyID] = true;
}

void InputManager::releaseKey(const unsigned int keyID)
{
    mKeyMap[keyID] = false;
}

bool InputManager::isKeyDown(const unsigned int keyID)
{
    auto it = mKeyMap.find(keyID);
    if (it != mKeyMap.end())
    {
        return it->second;
    }
    else
    {
        return false;
    }
}

bool InputManager::wasKeyDown(const unsigned int keyID)
{
    auto it = mPrevKeyMap.find(keyID);
    if (it != mPrevKeyMap.end())
    {
        return it->second;
    }
    else
    {
        return false;
    }
}

bool InputManager::isKeyPressed(const unsigned int keyID)
{
    if (isKeyDown(keyID) && wasKeyDown(keyID))
    {
        // Key is pressed
        return true;
    }
    return false;
}

void InputManager::setMouseCoords(const float x, const float y)
{
    mMouseCoords.x = x;
    mMouseCoords.y = y;
}