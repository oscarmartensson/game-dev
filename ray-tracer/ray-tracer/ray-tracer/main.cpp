#include <iostream>
#include <fstream>

#include "Vec3.h"

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

    // Write colors to file in the .ppm format
    for (int j = ny - 1; j >= 0; j--) 
    {
        for (int i = 0; i < nx; i++)
        {
            Vec3 col(static_cast<float>(i) / static_cast<float>(nx), static_cast<float>(j) / static_cast<float>(ny), 0.2f);
            //float r = static_cast<float>(i) / static_cast<float>(nx);
            //float g = static_cast<float>(j) / static_cast<float>(ny);
            //float b = 0.2f;
            int ir = static_cast<int>(255.99 * col[0]);
            int ig = static_cast<int>(255.99 * col[1]);
            int ib = static_cast<int>(255.99 * col[2]);

            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }

    return 0;
}