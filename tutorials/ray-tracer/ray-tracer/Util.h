#ifndef UTIL_H
#define UTIL_H

#include <random>

#include "Vec3.h"

// Returns a pseudo-random float in range [min, max], normal distributed
float randomFloat(float min, float max);

// Returns a random point inside a unit sphere (radius = 1)
Vec3 randomPointInUnitSphere();

// Returns a random point inside a unit disk, where the disk lies in the xy plane.
Vec3 randomPointInUnitDisk();

#endif // !UTIL_H

