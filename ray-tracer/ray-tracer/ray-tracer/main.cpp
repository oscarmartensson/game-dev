#include <iostream>
#include <fstream>

#include "Vec3.h"
#include "Ray.h"

// Decides if a sphere has been hit by a ray
// Solve equation :
// dot((A + t*B - C) ,(A + t*B - C)) = R*R
// This leads to following equation:
// t*t*dot(B, B) + 2*t*dot(B,A-C) + dot(A-C,A-C) - R*R = 0
// Solving this with the pq formula puts the constraint on the
// roots (in the square-root) that ( 2*dot(B,A-C) )^2 - 4 * dot(B, B) * dot(A-C,A-C) > 0
bool hitSphere(const Vec3& center, float radius, const Ray& r)
{
    Vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());    // dot(B, B)
    float b = 2.0f * dot(oc, r.direction());        // 2 * dot(B,A-C)
    float c = dot(oc, oc) - radius * radius;        // dot(A-C,A-C)
    float solutions = b * b - 4 * a * c;
    return solutions > 0.0f;
}

// Returns a color of different objects that were hit with ray
Vec3 color(const Ray& r)
{
    if (hitSphere(Vec3(0, 0, -1), 0.5, r))
    {
        // Red if sphere was hit with ray
        return Vec3(1, 0, 0);
    }

    // Background color if no object was hit
    Vec3 unitDir = unitVector(r.direction());
    float t = 0.5f * (unitDir.y() + 1);
    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main() 
{
    // Create a file and redirect std::cout to the file
    std::ofstream out("test.ppm");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    // x and y dimensions
    int nx = 200;
    int ny = 100;
    std::cout << "P3" << std::endl << nx << " " << ny << std::endl << 255 << std::endl;

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
            Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
            Vec3 col = color(r);

            int ir = static_cast<int>(255.99 * col[0]);
            int ig = static_cast<int>(255.99 * col[1]);
            int ib = static_cast<int>(255.99 * col[2]);

            // Write color to .ppm file
            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }

    return 0;
}