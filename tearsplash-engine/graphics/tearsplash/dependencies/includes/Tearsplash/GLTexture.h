// GLTexture.h

#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <GL/glew.h>

namespace Tearsplash
{

	struct GLTexture
	{
		GLuint id;
		int width;
		int height;
	};

}

#endif // !GLTEXTURE_H