#include <iostream>
#include <fstream>
#include <limits>

#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "HitableList.h"
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

int main() 
{
    // Create a file and redirect std::cout to the file
    std::ofstream out("scene.ppm");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    // x and y dimensions
    int nx = 200;
    int ny = 100;
    std::cout << "P3" << std::endl << nx << " " << ny << std::endl << 255 << std::endl;

    Hitable* list[2];
    list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f);
    Hitable* world = new HitableList(list,2);

    // Define the camera space
    Vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 2.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);

    // Write colors to file in the .ppm format
    for (int j = ny - 1; j >= 0; j--) 
    {
        for (int i = 0; i < nx; i++)
        {
            // Shoot rays over the scene
            float u = static_cast<float>(i) / static_cast<float>(nx);
            float v = static_cast<float>(j) / static_cast<float>(ny);
            Ray r(origin, lowerLeftCorner + u*horizontal + v*vertical);
            Vec3 col = color(r, world);

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