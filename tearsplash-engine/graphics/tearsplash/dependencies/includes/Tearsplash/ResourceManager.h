#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Tearsplash/TextureCache.h"

class ResourceManager
{
public:
	static GLTexture getTexture(std::string texturePath);
	
private:
	static TextureCache mTextureCache;
};

#endif // !RESOURCEMANAGER_H
