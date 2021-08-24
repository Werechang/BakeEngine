// Das Internet ist für uns alle Neuland.
#ifndef BAKEENGINE_MATH_H
#define BAKEENGINE_MATH_H


class Math {
public:
    constexpr static const float PI = 3.14159265;

    static float clamp(float min, float max, float value);
    static float toRadians(float angle);
};


#endif
