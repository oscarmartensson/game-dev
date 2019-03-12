#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Tearsplash/TextureCache.h"

namespace Tearsplash
{

	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache mTextureCache;
	};

}

#endif // !RESOURCEMANAGER_H
