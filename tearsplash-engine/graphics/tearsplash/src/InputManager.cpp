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

InputManager::InputManager() {}

InputManager::~InputManager() {}

void InputManager::pressKey(const unsigned int keyID)
{
    // First checks if keyID exists, if not, creates new entry
    mKeyMap[keyID] = true;
}

void InputManager::releaseKey(const unsigned int keyID)
{
    mKeyMap[keyID] = false;
}

bool InputManager::isKeyPressed(unsigned int keyID)
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