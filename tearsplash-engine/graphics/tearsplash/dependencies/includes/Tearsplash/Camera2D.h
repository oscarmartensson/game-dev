// Camera2D.h

#ifndef CAMERA2D_H
#define CAMERA2D_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Tearsplash
{

    class Camera2D
    {
    public:
        Camera2D();
        ~Camera2D();

        void init(int screenWidth, int screenHeight);
        void update();

        void setPosition(const glm::vec2& position) { mPosition = position; mNeedsMatrixUpdate = true; }
        void setScale(const float scale) { mScale = scale; mNeedsMatrixUpdate = true; }

        glm::vec2 getPosition() const { return mPosition; }
        glm::mat4 getCameraMatrix() const { return mCameraMatrix; }
        float getScale() const { return mScale; }


    private:
        float     mScale;
        int       mScreenWidth;
        int       mScreenHeight;
        bool      mNeedsMatrixUpdate;
        glm::vec2 mPosition;
        glm::mat4 mCameraMatrix;
        glm::mat4 mOrthoMatrix;

    };
}

#endif // !CAMERA2D_H
