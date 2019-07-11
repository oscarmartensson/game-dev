#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "Util.h"

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

    // The depth of field works as such that rays are shot from a virtual lens onto a focus plane (at distance focusDistance)

    // @param lookFrom      The position the camera should look from (origin).
    // @param lookAt        The position the target is located at.
    // @param vup           Vector defining the "up" direction of the camera at the target position.
    // @param vfov          The field of view angle of the camera, measured in degrees
    // @param aspec         Aspect ratio of the camera.
    // @param aperture      Size of a virtual aperture of the camera. Used for depth of field effect.
    // @param focusDistance At which distance away from the camera the focus of the camera should lie. Set -1 to disable.
    Camera(const Vec3& lookFrom, const Vec3& lookAt, const Vec3 vup, const float vfov, const float aspect, const float aperture, const float _focusDistance);
    ~Camera() {};
    Ray getRay(const float u, const float v);

private:
    Vec3 mOrigin;
    Vec3 mLowerLeftCorner;
    Vec3 mHorizontal;
    Vec3 mVertical;
    Vec3 u, v, w;
    float mLensRadius;
    float mFocusDistance;
};

#endif // !CAMERA_H
