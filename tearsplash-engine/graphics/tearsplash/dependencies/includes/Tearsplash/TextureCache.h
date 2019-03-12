#ifndef TEXTURECACHE_H
#define TEXTURECACHE_H

#include <map>
#include <string>
#include <Tearsplash/GLTexture.h>

namespace Tearsplash
{

	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string texturePath);

	private:
		std::map<std::string, GLTexture> mTextureMap;
	};

}

#endif // !TEXTURECACHE_H
