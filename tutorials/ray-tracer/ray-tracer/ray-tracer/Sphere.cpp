#include "Sphere.h"

bool Sphere::hit(const Ray& r, const float tMin, const float tMax, HitRecord& record) const
{
    Vec3 oc = r.origin() - center;

    float p = 2.0f * dot(oc, r.direction()) / dot(r.direction(), r.direction());
    float q = (dot(oc, oc) - radius * radius) / dot(r.direction(), r.direction());
    float discriminant = ((p*p) * 0.25f) - q;

    if (discriminant > 0.0f)
    { 
        // Sphere was hit
        float solution = -p * 0.5f - sqrt(discriminant);
        if (solution < tMax && solution > tMin)
        {
            // Solution is within tolerance boundaries
            record.t = solution;
            record.p = r.pointAtAbscissa(solution);
            record.normal = (record.p - center) / radius;
            record.materialPtr = materialPtr;
            return true;
        }

        // Check other solution
        solution = -p * 0.5f + sqrt(discriminant);
        if (solution < tMax && solution > tMin)
        {
            // Solution is within tolerance boundaries
            record.t = solution;
            record.p = r.pointAtAbscissa(solution);
            record.normal = (record.p - center) / radius;
            record.materialPtr = materialPtr;
            return true;
        }
    }
    // No hits on sphere
    return false;
}