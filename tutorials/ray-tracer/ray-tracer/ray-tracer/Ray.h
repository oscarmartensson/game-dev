#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray
{
public:
    Ray(const Vec3& a, const Vec3& b) : A(a), B(b) {};
    Ray() : A(Vec3(0,0,0)), B(Vec3(1,1,1)) {};
    ~Ray(){};

    // Methods
    Vec3 origin()    const { return A; }
    Vec3 direction() const { return B; }
    Vec3 pointAtAbscissa(float t) const { return A + t * B; }

private:
    Vec3 A;
    Vec3 B;
};

#endif // !RAY_H
