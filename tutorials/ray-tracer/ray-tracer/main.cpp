#include <iostream>
#include <fstream>
#include <limits>

#include "Ray.h"
#include "HitableList.h"
#include "Camera.h"
#include "Sphere.h"
#include "Material.h"

// ------Constants------
const float tolerance = 0.001f; // For getting rid of numerical error causing shadow acne

// x and y dimensions (pixels) of the output image
const int nx = 200;
const int ny = 100;

// Anti-aliasing samples
const int ns = 100;


// Returns a color of different objects that were hit with ray
Vec3 color(const Ray& r, Hitable* world, const int depth)
{
    HitRecord record;
    if (world->hit(r, tolerance, FLT_MAX, record))
    {
        Ray scattered;
        Vec3 attenuation;
        // Check to see what happens with ray depending on material
        // Set maximum depth to prevent infinite recursiveness
        if (depth < 50 && record.materialPtr->scatter(r, record, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return Vec3(0, 0, 0);
        }
    }
    else
    {
        // Background color if no object was hit
        Vec3 unitDir = unitVector(r.direction());
        float t = 0.5f * (unitDir.y() + 1);
        return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
    }
}

int main() 
{
    // Create a file and redirect std::cout to the file
    std::ofstream out("scene.ppm");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    std::cout << "P3" << std::endl << nx << " " << ny << std::endl << 255 << std::endl;

    Vec3 lookFrom = Vec3(3.0f, 3.0f, 2.0f);
    Vec3 lookAt = Vec3(0.0f, 0.0f, -1.0f);
    Vec3 upVector = Vec3(0.0f, 1.0f, 0.0f);
    float FOV = 20;
    float aperture = 2.0f;
    float distFocus = (lookFrom - lookAt).length();

    Camera camera(lookFrom, lookAt, upVector, FOV, static_cast<float>(nx) / static_cast<float>(ny), aperture, distFocus);

    // Builds a basic scene with some spheres of different materials and different positions.
    Hitable* list[4];
    list[0] = new Sphere(Vec3( 0.0f,  0.0f,   -1.0f), 0.5f,   new Lambertian(Vec3(0.8f, 0.3f, 0.3f)));
    list[1] = new Sphere(Vec3( 0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(Vec3(0.8f, 0.8f, 0.0f)));
    list[2] = new Sphere(Vec3( 1.0f,  0.0f,   -1.0f), 0.5f,   new Metal(     Vec3(0.8f, 0.6f, 0.2f), 0.3f));
    list[3] = new Sphere(Vec3(-1.0f,  0.0f,   -1.0f), -0.5f,  new Dielectric(1.5)); // Negative radius makes surface normals point inwards -> glass sphere look-like
    Hitable* world = new HitableList(list,4);


    // Write colors to file in the .ppm format
    for (int j = ny - 1; j >= 0; j--) 
    {
        for (int i = 0; i < nx; i++)
        {
            Vec3 col(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < ns; s++)
            {
                // Anti-aliasing
                // Run for ns additional times for each pixel. Sample in an area around the actual pixel, using
                // normal distributed pseudo-random numbers to get smoother edges on objects.
                float u = (static_cast<float>(i) + randomFloat(0.0f, 0.99999f)) / static_cast<float>(nx);
                float v = (static_cast<float>(j) + randomFloat(0.0f, 0.99999f)) / static_cast<float>(ny);
                Ray r = camera.getRay(u, v);
                col += color(r, world, 0);
            }
            
            col /= static_cast<float>(ns); // Average out since we ran ns more times per pixel to anti-alias
            col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2])); // Gamma correction with gamma 2 (i.e. x^(1/gamma)) since image viewer assumes some correction, otherwise very dark result
            int ir = static_cast<int>(255.99 * col[0]);
            int ig = static_cast<int>(255.99 * col[1]);
            int ib = static_cast<int>(255.99 * col[2]);

            // Write color to .ppm file
            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }

    for (int i = 0; i < 2; i++)
    {
        free(list[i]);
    }
    free(world);

    return 0;
}