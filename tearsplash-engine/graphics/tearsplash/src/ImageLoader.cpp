/**********************************************************************/
// File:	ImageLoader.cpp
// Summary:	Loads a PNG image and decodes it using the PicoPNG library.
//			Also binds the texture to an id and creates the texture using
//			OpenGL function calls.
//
// Version: Version 1.0 - 2018-08-26
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-26 File created
/**********************************************************************/

// Includes -------------------------
#include "ImageLoader.h"
#include "IOManager.h"
#include "PicoPNG.h"
#include "Errors.h"

// ----------------------------------
// Loads a PNG image at filePath using PicoPNG library decoder. Returns texture.
GLTexture ImageLoader::loadPNG(const std::string& filePath)
{
	// Init all texture values to zero
	GLTexture texture = {};
	std::vector<unsigned char> out;
	std::vector<unsigned char> in;
	unsigned long width, height;

	if (IOManager::readFileIntoBuffer(filePath, in) == false)
	{
		fatalError("Failed to load PNG file to buffer at path: " + filePath);
	}

	// Decode PNG using PicoPNG
	int errorCode = decodePNG(out, width, height, &(in[0]), in.size(), true);
	if (errorCode != 0)
	{
		fatalError("Decode PNG failed with error code: " + std::to_string(errorCode));
	}

	// Generate texture, bind it, and create a 2D image
	glGenTextures(1, &(texture.id));
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)&(out[0]));

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Generate the mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	return texture;
}