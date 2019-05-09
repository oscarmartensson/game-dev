#include <iostream>
#include <fstream>
#include <limits>

#include "Vec3.h"
#include "Ray.h"

// Decides where on a sphere a ray has been hit.
//
// Solve equation:
// dot((A + t*B - C) ,(A + t*B - C)) = R*R
//
// This leads to following equation:
// t*t*dot(B, B) + 2*t*dot(B,A-C) + dot(A-C,A-C) - R*R = 0
//
// Solving this with the pq formula:
// t1 = -p/2 + sqrt( (p/2)^2 - q ) 
// t2 = -p/2 - sqrt( (p/2)^2 - q )
// Here, p = 2*dot(B,A-C) / dot(B, B), and q = ( dot(A-C,A-C) - R*R) / dot(B, B)
//
// This puts the constraint on the discriminant (in the square-root of the pq formula) that: 
// sqrt( (p/2)^2 - q ) >= 0 =>
// ( (2*dot(B,A-C)) / (2 * dot(B, B)) )^2 - ( dot(A-C,A-C) - R*R ) / dot(B, B) >= 0 =>
// dot(B,A-C) * dot(B,A-C) - dot(B, B) * dot(A-C,A-C) - R*R >= 0
// for the real solutions, which are the ones we are interested in since non-real solutions means
// that the sphere was missed.
float hitSphere(const Vec3& center, float radius, const Ray& r)
{
    Vec3 oc = r.origin() - center;

    float p = 2.0f * dot(oc, r.direction()) / dot(r.direction(), r.direction());
    float q = (dot(oc, oc) - radius * radius) / dot(r.direction(), r.direction());
    float discriminant = ( (p*p) * 0.25f ) - q;

    if (discriminant < 0)
    {
        // No hits on sphere
        return -1.0;
    }
    else
    {
        // Sphere was hit
        return -p * 0.5f - sqrt(discriminant);
    }
    
}

// Returns a color of different objects that were hit with ray
Vec3 color(const Ray& r)
{
    float t = hitSphere(Vec3(0, 0, -1), 0.5f, r);

    if (t > 0.0f)
    {
        // If sphere front side (the side with normals pointing towards camera) was hit with rays, calculate normal and interpolate color
        Vec3 N = unitVector(r.pointAtAbscissa(t) - Vec3(0, 0, 0 -1));
        return 0.5f * Vec3(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    // Background color if no object was hit
    Vec3 unitDir = unitVector(r.direction());
    t = 0.5f * (unitDir.y() + 1);
    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
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