#ifndef MATERIAL_H
#define MATERIAL_H

#include "Util.h"
#include "Ray.h"
#include "Hitable.h"

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
    Metal(const Vec3& albedo) : mAlbedo(albedo) {};
    virtual bool scatter(const Ray& rayIn, HitRecord& record, Vec3& attenuation, Ray& rayScattered) const
    {
        Vec3 reflected = reflect(unitVector(rayIn.direction()), record.normal);
        rayScattered = Ray(record.p, reflected);
        attenuation = mAlbedo;
        return (dot(rayScattered.direction(), record.normal) > 0);
    }

    Vec3 mAlbedo;

private:
    // Reflects a ray against a plane with 100% reflectivity (mirror)
    Vec3 reflect(const Vec3& v, const Vec3& n) const
    {
        return v - 2 * dot(v, n) * n;
    }
};

#endif // !MATERIAL_H
