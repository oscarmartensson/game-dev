#ifndef MATERIAL_H
#define MATERIAL_H

#include "Util.h"
#include "Ray.h"
#include "Hitable.h"

// Reflects a ray against a plane with 100% reflectivity (mirror)
Vec3 reflect(const Vec3& v, const Vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

// Refracts a ray
// RefractionFactor is the refraction index of the incident ray divided by the refraction index of the refracted ray.
bool refract(const Vec3& v, const Vec3& n, const float refractionFactor, Vec3& refracted)
{
    Vec3 uv = unitVector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - refractionFactor * refractionFactor * (1.0f - dt * dt);
    if (discriminant > 0.0f)
    {
        refracted = refractionFactor * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    else
    {
        return false;
    }
}

// Approximation by Christophe Schlick to get more realistic reflection in dielectric materials
float schilckApproximation(const float cosine, float const refractionIndex)
{
    float r0 = (1.0f - refractionIndex) / (1 + refractionIndex);
    float r0_square = r0 * r0;
    return r0_square + (1 - r0_square) * pow((1 - cosine), 5);
}

class Material
{
public:

    virtual bool scatter(const Ray& rayIn, HitRecord& record, Vec3& attentuation, Ray& rayScattered) const = 0;

};

// Defines a lambertian material (diffuse)
class Lambertian : public Material
{
public:
    Lambertian(const Vec3& albedo) : mAlbedo(albedo) {};
    ~Lambertian() {};

    virtual bool scatter(const Ray& rayIn, HitRecord& record, Vec3& attenuation, Ray& rayScattered) const
    {
        Vec3 target = record.p + record.normal + randomPointInUnitSphere();
        rayScattered = Ray(record.p, target - record.p);
        attenuation = mAlbedo;
        return true;
    }

    Vec3 mAlbedo;
};

class Metal : public Material
{
public:
    Metal(const Vec3& albedo, const float fuzz) : mAlbedo(albedo)
    {
        if (fuzz <= 1.0f)
        {
            mFuzz = fuzz;
        }
        else
        {
            mFuzz = 1.0f;
        }
    };

    virtual bool scatter(const Ray& rayIn, HitRecord& record, Vec3& attenuation, Ray& rayScattered) const
    {
        Vec3 reflected = reflect(unitVector(rayIn.direction()), record.normal);
        rayScattered = Ray(record.p, reflected + mFuzz * randomPointInUnitSphere());
        attenuation = mAlbedo;
        return (dot(rayScattered.direction(), record.normal) > 0);
    }

    Vec3 mAlbedo;

private:
    float mFuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(float refractionIndex) : mRefractionIndex(refractionIndex) {};

    virtual bool scatter(const Ray& rayIn, HitRecord& record, Vec3& attenuation, Ray& rayScattered) const
    {
        // Attenuation of dielectric is always 1.
        attenuation = Vec3(1.0f, 1.0f, 1.0f);

        Vec3 outwardNormal;
        float refractionFactor;
        float cosine;
        float rayInDirectionDiv = 1.0f / rayIn.direction().length();

        if (dot(rayIn.direction(), record.normal) > 0.0f)
        {
            outwardNormal = -record.normal;
            refractionFactor = mRefractionIndex;
            cosine = mRefractionIndex * dot(rayIn.direction(), record.normal) * rayInDirectionDiv;
        }
        else
        {
            outwardNormal = record.normal;
            refractionFactor = 1.0f / mRefractionIndex;
            cosine = -dot(rayIn.direction(), record.normal) * rayInDirectionDiv;
        }

        float reflectionProbability;
        Vec3 refracted;
        Vec3 reflected = reflect(rayIn.direction(), record.normal);
        if (refract(rayIn.direction(), outwardNormal, refractionFactor, refracted))
        {
            reflectionProbability = schilckApproximation(cosine, mRefractionIndex);
        }
        else
        {
            //rayScattered = Ray(record.p, reflected);
            reflectionProbability = 1.0f;
        }

        // Check if scattered ray should be reflected or refracted
        if (randomFloat(0.0f, 0.9999f) < reflectionProbability)
        { 
            rayScattered = Ray(record.p, reflected);
        }
        else
        {
            rayScattered = Ray(record.p, refracted);
        }
        return true;
    }

private:
    float mRefractionIndex;
};

#endif // !MATERIAL_H
