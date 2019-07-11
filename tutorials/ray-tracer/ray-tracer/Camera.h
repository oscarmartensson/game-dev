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
    // Convention: (0, 1, 0) is defined as "up".
    //      +Y
    //      |
    //      |
    //      |______+X
    //     /       
    //    /
    //   +Z

    // @param lookFrom      The position the camera should look from (origin).
    // @param lookAt        The position the target is located at.
    // @param vup           Vector defining the "up" direction of the camera at the target position.
    // @param vfov          The field of view angle of the camera, measured in degrees
    // @param aspec         Aspect ratio of the camera.
    Camera(const Vec3& lookFrom, const Vec3& lookAt, const Vec3 vup, const float vfov, const float aspect) : origin(Vec3(0.0f,0.0f,0.0f))
    {
        float theta = vfov * PI_DOUBLE / 180.0;
        float halfHeight = tan(theta * 0.5f);
        float halfWidth = aspect * halfHeight;
        origin = lookFrom;

        // Create vectors for defining the plane at which the camera origin is centered
        Vec3 w = unitVector(lookFrom - lookAt);
        Vec3 u = unitVector( cross(vup, w));
        Vec3 v = unitVector( cross(w, u));

        //lowerLeftCorner = Vec3(-halfWidth, -halfHeight, -1.0);
        lowerLeftCorner = origin - halfWidth * u - halfHeight * v - w;
        horizontal = 2.0f * halfWidth * u;
        vertical = 2.0f * halfHeight * v;
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
