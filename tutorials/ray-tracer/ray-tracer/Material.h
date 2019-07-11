#ifndef MATERIAL_H
#define MATERIAL_H

#include "Util.h"
#include "Ray.h"
#include "Hitable.h"

// Reflects a ray against a plane with 100% reflectivity (mirror)
Vec3 reflect(const Vec3& v, const Vec3& n);

// Refracts a ray
// RefractionFactor is the refraction index of the incident ray divided by the refraction index of the refracted ray.
bool refract(const Vec3& v, const Vec3& n, const float refractionFactor, Vec3& refracted);

// Approximation by Christophe Schlick to get more realistic reflection in dielectric materials
float schilckApproximation(const float cosine, float const refractionIndex);

// Base class for materials
class Material
{
public:
    // Must be overridden when inherited.
    // Determines how the incoming rays should scatter against the material.
    virtual bool scatter(const Ray& rayIn, HitRecord& record, Vec3& attentuation, Ray& rayScattered) const = 0;

};

// Defines a lambertian material (diffuse)
class Lambertian : public Material
{
public:
    Lambertian(const Vec3& albedo) : mAlbedo(albedo) {};
    ~Lambertian() {};

    // Determines how the incoming rays should scatter against the material.
    virtual bool scatter(const Ray& rayIn, HitRecord& record, Vec3& attenuation, Ray& rayScattered) const;

    Vec3 mAlbedo;
};

// Defines a metal material
class Metal : public Material
{
public:
    Metal(const Vec3& albedo, const float fuzz);

    // Determines how the incoming rays should scatter against the material.
    virtual bool scatter(const Ray& rayIn, HitRecord& record, Vec3& attenuation, Ray& rayScattered) const;

    Vec3 mAlbedo;

private:
    float mFuzz;
};

// Defines a dielectric material
class Dielectric : public Material
{
public:
    Dielectric(float refractionIndex) : mRefractionIndex(refractionIndex) {};

    // Determines how the incoming rays should scatter against the material.
    virtual bool scatter(const Ray& rayIn, HitRecord& record, Vec3& attenuation, Ray& rayScattered) const;

private:
    float mRefractionIndex;
};

#endif // !MATERIAL_H
