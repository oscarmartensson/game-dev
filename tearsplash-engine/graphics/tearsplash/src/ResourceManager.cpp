/**********************************************************************/
// File:	ResourceManager.cpp
// Summary:	-
//
// Version: Version 1.0 - 2018-09-02
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-02 File created
/**********************************************************************/

#include "Tearsplash/ResourceManager.h"

// ----------------------------------
// Initialize static variables
TextureCache ResourceManager::mTextureCache{};


GLTexture ResourceManager::getTexture(std::string texturePath)
{
	return mTextureCache.getTexture(texturePath);
}