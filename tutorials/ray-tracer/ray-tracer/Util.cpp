#include "Util.h"


float randomFloat(float min, float max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

Vec3 randomPointInUnitSphere()
{
    Vec3 point;
    do {
        point = 2.0f * Vec3(randomFloat(0.0f, 0.9999f), randomFloat(0.0f, 0.9999f), randomFloat(0.0f, 0.9999f)) - Vec3(1.0f, 1.0f, 1.0f);
    } while (point.squareLength() >= 1.0f);
    return point;
}

Vec3 randomPointInUnitDisk()
{
    Vec3 point;
    do {
        point = 2.0f * Vec3(randomFloat(0.0f, 0.9999f), randomFloat(0.0f, 0.9999f), 0.0f) - Vec3(1.0f, 1.0f, 0.0f);
    } while (point.squareLength() >= 1.0f);
    return point;
}
