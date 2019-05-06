#include "Vec3.h"


Vec3& Vec3::operator+=(const Vec3& v) 
{
    e[0] += v.e[0];
    e[1] += v.e[2];
    e[2] += v.e[2];
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[2];
    e[2] -= v.e[2];
    return *this;
}

Vec3& Vec3::operator*=(const Vec3& v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[2];
    e[2] *= v.e[2];
    return *this;
}

Vec3& Vec3::operator/=(const Vec3& v)
{
    e[0] /= v.e[0];
    e[1] /= v.e[2];
    e[2] /= v.e[2];
    return *this;
}

Vec3& Vec3::operator*=(const float t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

Vec3& Vec3::operator/=(const float t)
{
    float div = 1.0f / t;
    e[0] *= div;
    e[1] *= div;
    e[2] *= div;
    return *this;
}

inline float Vec3::dot(const Vec3& v) const
{
    return v[0] * e[0] + v[1] * e[1] + v[2] * e[2];
}

inline Vec3 Vec3::cross(const Vec3& v) const
{
    return Vec3( (v[1] * e[2] - v[2] * e[1]),
                -(v[0] * e[2] - v[2] * e[0]),
                 (v[0] * e[1] - v[1] * e[0]) );
}

std::istream& Vec3::operator>>(std::istream& is)
{
    is >> e[0] >> e[1] >> e[2];
    return is;
}

inline std::ostream& Vec3::operator<<(std::ostream& os)
{
    os << e[0] << e[1] << e[2];
    return os;
}

// Converts the vector to a unit vector (normalized)
void Vec3::makeUnitVector()
{
    float k = 1 / length();
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}