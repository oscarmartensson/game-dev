#ifndef SPHERE_H
#define SPHERE_H

#include "Hitable.h"

class Sphere : public Hitable
{
public:
    Sphere() : center(Vec3(0,0,0)), radius(1.0f), materialPtr(nullptr) {};
    Sphere(const Vec3 c, const float r, Material* mat) : center(c), radius(r), materialPtr(mat) {};
    virtual ~Sphere()
    { 
        if (materialPtr != nullptr)
        {
            delete(materialPtr);
        }
    };

    // Decides where on a sphere a ray has been hit.
    //
    // Solve equation:
    // dot((A + t*B - C) ,(A + t*B - C)) = R*R
    //
    // This leads to following equation:
    // t*t*dot(B, B) + 2*t*dot(B,A-C) + dot(A-C,A-C) - R*R = 0
    //
    // Solving this with the pq formula:
    // t1 = -p/2 + sqrt( (p/2)^2 - q ) 
    // t2 = -p/2 - sqrt( (p/2)^2 - q )
    // Here, p = 2*dot(B,A-C) / dot(B, B), and q = ( dot(A-C,A-C) - R*R) / dot(B, B)
    //
    // This puts the constraint on the discriminant (in the square-root of the pq formula) that: 
    // sqrt( (p/2)^2 - q ) >= 0 =>
    // ( (2*dot(B,A-C)) / (2 * dot(B, B)) )^2 - ( dot(A-C,A-C) - R*R ) / dot(B, B) >= 0 =>
    // dot(B,A-C) * dot(B,A-C) - dot(B, B) * dot(A-C,A-C) - R*R >= 0
    // for the real solutions, which are the ones we are interested in since non-real solutions means
    // that the sphere was missed.
    virtual bool hit(const Ray& r, const float tMin, const float tMax, HitRecord& record) const;

private:
    Vec3 center;
    float radius;
    Material* materialPtr;
};

#endif // !SPHERE_H