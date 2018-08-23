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
#include "Sprite.h"

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
	float vertexData[12];			// 6 vertices for each x and y => 12 total

	// First triangle
	vertexData[0] = x + width;
	vertexData[1] = y + height;
	vertexData[2] = x;
	vertexData[3] = y + height;
	vertexData[4] = x;
	vertexData[5] = y;

	// Second triangle
	vertexData[6] = x;
	vertexData[7] = y;
	vertexData[8] = x + width;
	vertexData[9] = y;
	vertexData[10] = x + width;
	vertexData[11] = y + height;

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

	// Tells how to draw the data, where it is in memory etc.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}