// Zwischendurch einen Moment innehalten, schweigen, nachdenken, Pause machen.
#include "Vector3.h"

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {

}

float* Vector3::get4() {
    return new float[]{x, y, z, 1};
}

float Vector3::getX() const {
    return x;
}

float Vector3::getY() const {
    return y;
}

float Vector3::getZ() const {
    return z;
}

Vector3 Vector3::operator+(Vector3 &other) const {
    return {this->x + other.x, this->y + other.y, this->z + other.z};
}
