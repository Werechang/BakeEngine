//
// Created by tom on 11.08.2021.
//

#include "Vector3.h"

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {

}

float* Vector3::get4() {
    return new float[]{x, y, z, 0};
}