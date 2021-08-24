// Das Internet ist für uns alle Neuland.
#include "Math.h"

float Math::clamp(float min, float max, float value) {
    if (value <= min) return min;
    else if (value >= max) return max;
    else return value;
}

float Math::toRadians(float angle) {
    return angle*(PI/180);
}
