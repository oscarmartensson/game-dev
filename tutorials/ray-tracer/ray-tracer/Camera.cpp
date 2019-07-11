#include "Camera.h"


Camera::Camera(const Vec3& lookFrom, const Vec3& lookAt, const Vec3 vup, const float vfov, const float aspect, const float aperture, const float _focusDistance) :
    mLensRadius(aperture * 0.5f),
    mFocusDistance(_focusDistance)
{
    float theta = static_cast<float>(vfov * PI_DOUBLE / 180.0);
    float halfHeight = tan(theta * 0.5f);
    float halfWidth = aspect * halfHeight;
    mOrigin = lookFrom;

    // Create vectors for defining the plane at which the camera origin is centered
    w = unitVector(lookFrom - lookAt);
    u = unitVector(cross(vup, w));
    v = unitVector(cross(w, u));

    if (mFocusDistance < 0.0f)
    {
        // Depth of field is disabled
        mLowerLeftCorner = mOrigin - halfWidth * u - halfHeight * v - w;
        mHorizontal = 2.0f * halfWidth * u;
        mVertical = 2.0f * halfHeight * v;
    }
    else
    {
        //lowerLeftCorner = Vec3(-halfWidth, -halfHeight, -1.0);
        mLowerLeftCorner = mOrigin - halfWidth * mFocusDistance * u - halfHeight * mFocusDistance * v - mFocusDistance * w;
        mHorizontal = 2.0f * halfWidth * mFocusDistance * u;
        mVertical = 2.0f * halfHeight * mFocusDistance * v;
    }

};

Ray Camera::getRay(const float s, const float t)
{
    if (mFocusDistance < 0.0f)
    {
        // Depth of field is disabled
        return Ray(mOrigin, mLowerLeftCorner + s * mHorizontal + t * mVertical - mOrigin);
    }
    Vec3 rd = mLensRadius * randomPointInUnitDisk();
    Vec3 offset = u * rd.x() + v * rd.y();
    return Ray(mOrigin + offset, mLowerLeftCorner + s * mHorizontal + t * mVertical - mOrigin - offset);
}