// ImageLoader.h

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include "GLTexture.h"
#include "string"

namespace Tearsplash
{

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(const std::string& filePath);
	};

}
#endif // !IMAGELOADER_H
