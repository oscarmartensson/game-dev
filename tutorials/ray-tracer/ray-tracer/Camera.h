#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

static const double PI_DOUBLE = 3.14159265358979323846;

class Camera
{
public:
    // Define the camera space traversal. Note that -Z is the direction we're shooting
    // the rays towards and what is supposed to be thought of as "in front of" the camera.
    // The camera is initially positioned at the xy plane at z=-1
    //      +Y
    //      |
    //      |
    //      |______+X
    //     /       
    //    /
    //   +Z

    // @param vfov          The field of view angle of the camera, measured in degrees
    // @param aspec         Aspect ratio of the camera.
    Camera(const float vfov, const float aspect) : origin(Vec3(0.0f,0.0f,0.0f))
    {
        float theta = vfov * PI_DOUBLE / 180.0;
        float halfHeight = tan(theta * 0.5f);
        float halfWidth = aspect * halfHeight;
        lowerLeftCorner = Vec3(-halfWidth, -halfHeight, -1.0);
        horizontal = Vec3(2 * halfWidth, 0.0f, 0.0f);
        vertical = Vec3(0.0f, 2 * halfHeight, 0.0f);
    };
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
