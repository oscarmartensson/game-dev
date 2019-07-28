// Vertex.h

#ifndef VERTEX_H
#define VERTEX_H

#include <cstdint>
#include <GL/glew.h>

namespace Tearsplash
{

	// Position information
	struct Position
	{
		float x;
		float y;
	};

	// Color information
	struct ColorRGBA8
	{
        ColorRGBA8() : r(255), g(0), b(255), a(1) { 
            // Initialize to Magenta so that the color stands out if not properly set.
        }

        ColorRGBA8(const GLbyte _r, const GLbyte _g, const GLbyte _b, const GLbyte _a) : r(_r), g(_g), b(_b), a(_a) {}

		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

	// UV information
	struct UV
	{
		float u;
		float v;
	};

	// Vertex data information
	struct Vertex
	{
		// Total of 12 bytes

		// Vertex position information
		Position position;

		// Vertex color information
        ColorRGBA8 color;

		// Sprite UV information
		UV uv;

		// Function for setting color
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		// Function for setting UV
		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		// Function for setting position
		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}
	};

}

#endif // !VERTEX_H
