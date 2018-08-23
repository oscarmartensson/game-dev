// Sprite.h

#ifndef SPRITE_H
#define SPRITE_H

#include <GL/glew.h>

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height); // Input in screen coordinates
	void draw();

private:
	float	mX;
	float	mY;
	float	mWidth;
	float	mHeight;
	GLuint	mVboID;

};

#endif // !SPRITE_H