// Das Internet ist für uns alle Neuland.
#pragma once

#include <cstddef>

constexpr static const float PI = 3.14159265;

float clamp(float min, float max, float value);

float toRadians(float angle);

template<size_t size>
void generatePerlinNoise(float (&noise)[size][size], int seed);

template<size_t size>
void generateWorleyNoise(float (&noise)[size][size], int seed);
