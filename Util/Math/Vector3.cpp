// Zwischendurch einen Moment innehalten, schweigen, nachdenken, Pause machen.
#include "Vector3.h"

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {

}

Vector3 Vector3::cross(Vector3 &other) {
    return {this->y * other.z - this->z * other.y,
            this->z * other.x - this->x * other.z,
            this->x * other.y - this->y * other.x};
}


Vector3 Vector3::operator+(Vector3 &other) const {
    return {this->x + other.x, this->y + other.y, this->z + other.z};
}

void Vector3::normalize() {
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}
