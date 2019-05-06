#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

// Description of a 3 element array and opreations that can be performed on it
class Vec3
{
public:
    Vec3() { e[0] = 0.0f; e[1] = 0.0f; e[2] = 0.0f; };
    Vec3(const Vec3& v) { e[0] = v[0]; e[1] = v[1]; e[2] = v[2]; };
    Vec3(const float e0, const float e1, const float e2) { e[0] = e0; e[1] = e1; e[2] = e2; };
    ~Vec3() {};

    // Methods
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const Vec3& operator+() const { return *this; }
    inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](const int i) const { return e[i]; }
    inline float& operator[](const int i) { return e[i]; }

    inline Vec3& operator+=(const Vec3& v);
    inline Vec3& operator-=(const Vec3& v);
    inline Vec3& operator*=(const Vec3& v);
    inline Vec3& operator/=(const Vec3& v);
    inline Vec3& operator*=(const float t);
    inline Vec3& operator/=(const float t);

    inline float dot(const Vec3& v) const;
    inline Vec3 cross(const Vec3& v) const;

    inline std::istream& operator>>(std::istream& is);
    inline std::ostream& operator<<(std::ostream& is);

    inline float length() const {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); // euclidean norm
    }
    inline float squareLength() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    inline void makeUnitVector();

    // Variables
    float e[3];
};

// -------Global Vec3 operator overrides-------

inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    return Vec3( v1.e[0] + v2.e[0],
                 v1.e[1] + v2.e[1],
                 v1.e[2] + v2.e[2] );
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
    return Vec3( v1.e[0] - v2.e[0],
                 v1.e[1] - v2.e[1],
                 v1.e[2] - v2.e[2] );
}

// Note! this is element wise multiplication and not real vector multiplication
inline Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
    return Vec3( v1.e[0] * v2.e[0],
                 v1.e[1] * v2.e[1],
                 v1.e[2] * v2.e[2] );
}

// Note! this is element wise division and not real vector inverse
inline Vec3 operator/(const Vec3& v1, const Vec3& v2)
{
    return Vec3( v1.e[0] / v2.e[0],
                 v1.e[1] / v2.e[1],
                 v1.e[2] / v2.e[2] );
}

inline Vec3 operator*(const Vec3& v, const float t)
{
    return Vec3( v.e[0] * t,
                 v.e[1] * t,
                 v.e[2] * t );
}

inline Vec3 operator*(const float t, const Vec3& v)
{
    return Vec3( v.e[0] * t,
                 v.e[1] * t,
                 v.e[2] * t );
}

inline Vec3 operator/(const Vec3& v, const float t)
{
    float k = 1.0f / t;
    return Vec3( v.e[0] * k,
                 v.e[1] * k,
                 v.e[2] * k );
}

// Returns a normalized version of vector v
inline Vec3 unitVector(Vec3 v)
{
    return v / v.length();
}

// Dot product between two 3 element vectors
inline float dot(const Vec3& v1, const Vec3& v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v2[2] * v2[2];
}

#endif // !VEC3_H

