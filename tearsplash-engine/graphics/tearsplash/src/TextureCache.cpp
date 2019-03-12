/**********************************************************************/
// File:	TextureCache.cpp
// Summary:	-
//
// Version: Version 1.0 - 2018-09-02
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-02 File created
/**********************************************************************/

#include <iostream>
#include "Tearsplash/TextureCache.h"
#include "Tearsplash/ImageLoader.h"

using namespace Tearsplash;

// ----------------------------------
// Default constructor
TextureCache::TextureCache()
{
}
// ----------------------------------
// Default destructor
TextureCache::~TextureCache()
{
	// Do nothing
}

// ----------------------------------
// Returns a GLTexture from the map structure if it exists
GLTexture TextureCache::getTexture(std::string texturePath)
{
	// auto replaces std::map<std::string, GLTexture>::iterator
	auto mit = mTextureMap.find(texturePath);

	if (mit == mTextureMap.end())
	{
		// Texture is not in map. Load png from path. loadPNG checks for fatal errors.
		GLTexture newText = ImageLoader::loadPNG(texturePath);

		// Add a new texture to the map
		std::pair<std::string, GLTexture> newPair(texturePath, newText);
		mTextureMap.insert(newPair);

		std::cout << "Loaded texture!" << std::endl;
		return newText;
	}

	std::cout << "Loaded cached texture!" << std::endl;

	// Returns second element in pair, which is GLTexture
	return mit->second;
}