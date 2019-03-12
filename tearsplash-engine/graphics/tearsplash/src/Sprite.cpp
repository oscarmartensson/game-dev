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
#include "Tearsplash/Sprite.h"
#include "Tearsplash/Vertex.h"
#include "Tearsplash/ResourceManager.h"

using namespace Tearsplash;

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
void Sprite::init(float x, float y, float width, float height, std::string texturePath)
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

	// Error is handled in function
	mTexture = ResourceManager::getTexture(texturePath);

	// Screen coordinate space vetex data
	Vertex vertexData[6];			// 6 vertices for each x and y => 12 total

	// First triangle positions
	vertexData[0].setPosition(x + width, y + height);
	vertexData[1].setPosition(x, y + height);
	vertexData[2].setPosition(x, y);

	// Second triangle positions
	vertexData[3].setPosition(x, y);
	vertexData[4].setPosition(x + width, y);
	vertexData[5].setPosition(x + width, y + height);

	// Set color for vertices
	for (int i = 0; i < 6; i++)
	{
		vertexData[i].setColor(255, 0, 255, 255);
	}

	vertexData[1].setColor(0, 255, 0, 255);

	// Set UV data
	vertexData[0].setUV(1.0f, 1.0f);
	vertexData[1].setUV(0.0f, 1.0f);
	vertexData[2].setUV(0.0f, 0.0f);
	vertexData[3].setUV(0.0f, 0.0f);
	vertexData[4].setUV(1.0f, 0.0f);
	vertexData[5].setUV(1.0f, 1.0f);

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
	// Bind texture. Don't unbind later since there might be more objects using same texture
	glBindTexture(GL_TEXTURE_2D, mTexture.id);

	// Bind buffer for drawing
	glBindBuffer(GL_ARRAY_BUFFER, mVboID);
	
	// Zeroth index, only vertex data
	glEnableVertexAttribArray(0);

	// Position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// Color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color)); // GL_TRUE == wants to normalize colors to [0,1]
	// UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}