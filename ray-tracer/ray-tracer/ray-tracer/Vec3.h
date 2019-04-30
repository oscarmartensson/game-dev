#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

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

    // TODO: implement from here...
    inline Vec3& operator+=(const Vec3& v);
    inline Vec3& operator-=(const Vec3& v);
    inline Vec3& operator*=(const Vec3& v);
    inline Vec3& operator/=(const Vec3& v);
    inline Vec3& operator*=(const float t);
    inline Vec3& operator/=(const float t);

    //inline Vec3 operator*(const float t, const Vec3& v);
    inline Vec3& operator/(const float t);
    inline Vec3& operator+(const Vec3& v);
    inline Vec3& operator-(const Vec3& v);
    inline Vec3& operator*(const Vec3& v);
    inline Vec3& operator/(const Vec3& v);
    // TODO: ... to here

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

#endif // !VEC3_H

