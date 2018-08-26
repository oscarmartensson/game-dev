/**********************************************************************/
// File:	Sprite.cpp
// Summary:	This is the class that runs the main and most high level
//			portion of the game. Here, things are initialized and run.
//
// Version: Version 1.0 - 2018-08-21
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-21 File created
/**********************************************************************/

// Includes -------------------------
#include <cstddef>
#include "Sprite.h"
#include "Vertex.h"

// ----------------------------------
// Default constructor
Sprite::Sprite() : mVboID( (GLuint)0)
{
	// Initialize member variables through MIL
}

// ----------------------------------
// Default destructor
Sprite::~Sprite()
{
	// Free buffer if it's initialized
	if (mVboID != 0)
	{
		glDeleteBuffers(1, &mVboID);
	}
}

// ----------------------------------
// Initializes a sprite with position x and y, 
// and sides width and height. Input in screen coordinates!
void Sprite::init(float x, float y, float width, float height)
{
	mX		= x;
	mY		= y;
	mWidth	= width;
	mHeight = height;

	// VBO not created yet
	if (mVboID == 0)
	{
		glGenBuffers(1, &mVboID);	// Generate buffer
	}

	// Screen coordinate space vetex data
	Vertex vertexData[6];			// 6 vertices for each x and y => 12 total

	// First triangle positions
	vertexData[0].position.x = x + width;
	vertexData[0].position.y = y + height;
	vertexData[1].position.x = x;
	vertexData[1].position.y = y + height;
	vertexData[2].position.x = x;
	vertexData[2].position.y = y;

	// Second triangle positions
	vertexData[3].position.x = x;
	vertexData[3].position.y = y;
	vertexData[4].position.x = x + width;
	vertexData[4].position.y = y;
	vertexData[5].position.x = x + width;
	vertexData[5].position.y = y + height;

	// Set color for vertices
	for (int i = 0; i < 6; i++)
	{
		vertexData[i].color.r = 255;
		vertexData[i].color.g = 0;
		vertexData[i].color.b = 255;
		vertexData[i].color.a = 255;
	}

	vertexData[1].color.r = 0;
	vertexData[1].color.g = 255;
	vertexData[1].color.b = 0;
	vertexData[1].color.a = 255;

	// Bind buffer and upload buffer data
	glBindBuffer(GL_ARRAY_BUFFER, mVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// Unbind buffer once finished
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ----------------------------------
// Draw sprite
void Sprite::draw()
{
	// Bind buffer for drawing
	glBindBuffer(GL_ARRAY_BUFFER, mVboID);
	
	// Zeroth index, only vertex data
	glEnableVertexAttribArray(0);

	// Position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// Color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color)); // GL_TRUE == wants to normalize colors to [0,1]

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}