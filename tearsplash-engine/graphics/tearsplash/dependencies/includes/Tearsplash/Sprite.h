// Sprite.h

#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <GL/glew.h>
#include "Tearsplash/GLTexture.h"

namespace Tearsplash
{

	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath); // Input in screen coordinates
		void draw();

	private:
		float		mX;
		float		mY;
		float		mWidth;
		float		mHeight;
		GLuint		mVboID;
		GLTexture	mTexture;
	};

}

#endif // !SPRITE_H