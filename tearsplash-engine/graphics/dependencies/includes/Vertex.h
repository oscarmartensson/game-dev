// Vertex.h

#ifndef VERTEX_H
#define VERTEX_H

#include <cstdint>

// Position information
struct Position
{
	float x;
	float y;
};

// Color information
struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

struct Vertex
{
	// Total of 12 bytes

	// Vertex position information
	Position position;

	// Vertex color information
	 Color color;
};



#endif // !VERTEX_H
