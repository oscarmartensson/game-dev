#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "TextureCache.h"

static class ResourceManager
{
public:
	static GLTexture getTexture(std::string texturePath);
	
private:
	static TextureCache mTextureCache;
};

#endif // !RESOURCEMANAGER_H
