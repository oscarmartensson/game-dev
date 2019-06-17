#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

class Camera
{
public:
    // Define the camera space traversal. Note that -Z is the direction we're shooting
    // the rays towards and what is supposed to be thought of as "in front of" the camera.
    //      +Y
    //      |
    //      |
    //      |______+X
    //     /       
    //    /
    //   +Z
    Camera() : origin(Vec3(0.0f,0.0f,0.0f)), lowerLeftCorner(Vec3(-2.0f, -1.0f, -1.0f)), horizontal(Vec3(4.0f, 0.0f, 0.0f)), vertical(Vec3(0.0f, 2.0f, 0.0f)) {};
    ~Camera() {};
    Ray getRay(const float u, const float v)
    {
        return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
    }

private:
    Vec3 origin;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
};

#endif // !CAMERA_H
