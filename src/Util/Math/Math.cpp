// Das Internet ist für uns alle Neuland.
#include "Math.h"

float clamp(float min, float max, float value) {
    if (value <= min) return min;
    else if (value >= max) return max;
    else return value;
}

float toRadians(float angle) {
    return angle*(PI/180);
}

template<size_t size>
void generatePerlinNoise(float (&noise)[size][size], int seed) {
    // Write perlin noise to noise ([rows][columns])
}

template<size_t size>
void generateWorleyNoise(float (&noise)[size][size], int seed) {
    // Write worley noise to noise ([rows][columns])
}