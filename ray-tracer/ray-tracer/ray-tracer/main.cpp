#include <iostream>
#include <fstream>
#include <limits>
#include <random>

#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Camera.h"
#include "Sphere.h"

// Returns a color of different objects that were hit with ray
Vec3 color(const Ray& r, Hitable* world)
{
    HitRecord record;
    if (world->hit(r, 0.0f, FLT_MAX, record))
    {
        // If sphere front side (the side with normals pointing towards camera) was hit with rays, calculate normal and interpolate color
        return 0.5f * Vec3(record.normal.x() + 1, record.normal.y() + 1, record.normal.z() + 1);
    }
    else
    {
        // Background color if no object was hit
        Vec3 unitDir = unitVector(r.direction());
        float t = 0.5f * (unitDir.y() + 1);
        return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
    }
}

// Returns a pseudo-random float in range [min, max], normal distributed
float randomFloat(float min, float max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<> distribution(min, max);
    return distribution(generator);
}

int main() 
{
    // Create a file and redirect std::cout to the file
    std::ofstream out("scene.ppm");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    // x and y dimensions
    const int nx = 200;
    const int ny = 100;
    const int ns = 100;
    std::cout << "P3" << std::endl << nx << " " << ny << std::endl << 255 << std::endl;

    Hitable* list[2];
    list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f);
    Hitable* world = new HitableList(list,2);

    Camera camera;


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
                col += color(r, world);
            }
            
            col /= static_cast<float>(ns); // Average out since we ran ns more times per pixel to anti-alias
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