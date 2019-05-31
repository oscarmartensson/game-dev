#ifndef HITABLE_H
#define HITABLE_H

#include "Ray.h"

class Material;

struct HitRecord
{
    float t;
    Vec3  p;
    Vec3  normal;
    Material *materialPtr;
};

class Hitable {
public:
    virtual bool hit(const Ray& r, const float tMin, const float tMax, HitRecord& record) const = 0;
};

#endif // !HITABLE_H

