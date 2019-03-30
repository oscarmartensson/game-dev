/**********************************************************************/
// File:	Camera2D.cpp
// Summary:	This is a 2D camera that can be used to view the scene in the
//          engine.
//
// Version: Version 1.0 - 2018-03-12
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-03-12 File created
/**********************************************************************/

// Includes -------------------------
#include "Tearsplash/Camera2D.h"

using namespace Tearsplash;

Camera2D::Camera2D() : mPosition(0, 0), mCameraMatrix(1.0), mScale(1.0f), mNeedsMatrixUpdate(true), mScreenWidth(480), mScreenHeight(480), mOrthoMatrix(0.0f)
{
    // Create identity matrix
    mCameraMatrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}


Camera2D::~Camera2D() 
{}

void Camera2D::init(int screenWidth, int screenHeight)
{
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;
    mOrthoMatrix = glm::ortho(0.0f, static_cast<float>(mScreenWidth), 0.0f, static_cast<float>(mScreenHeight));
}

void Camera2D::update()
{
    if (mNeedsMatrixUpdate)
    {
        // Adjust focal point for scaling
        glm::vec3 translate(-mPosition.x + mScreenWidth * 0.5 , -mPosition.y + mScreenHeight * 0.5, 0.0f);
        glm::vec3 scale(mScale, mScale, 0.0f);

        // Translate and scale camera
        mCameraMatrix = glm::translate(mOrthoMatrix, translate);
        mCameraMatrix = glm::scale(glm::mat4(1.0f), scale) * mCameraMatrix;

        mNeedsMatrixUpdate = false;
    }
}

// This function converts screen coordinates to engine world coordinates
glm::vec2 Camera2D::convertScreen2World(glm::vec2& screenCoords)
{
    // Invert Y direction
    screenCoords.y = mScreenHeight - screenCoords.y;
    // Convert origin to middle of screen
    screenCoords -= glm::vec2(mScreenWidth >> 1, mScreenHeight >> 1);
    // Take zooming (scale) into account
    screenCoords /= mScale;
    // Translate
    screenCoords += mPosition;

    return screenCoords;
}

