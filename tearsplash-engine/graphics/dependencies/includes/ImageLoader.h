// ImageLoader.h

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "GLTexture.h"
#include "string"

class ImageLoader
{
public:
	GLTexture loadPNG(const std::string& filePath);
};
#endif // !IMAGELOADER_H
