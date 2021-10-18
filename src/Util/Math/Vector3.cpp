// Zwischendurch einen Moment innehalten, schweigen, nachdenken, Pause machen.
#include "Vector3.h"
#include <cmath>

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z), magnitude(-1.0f) {
}

Vector3 Vector3::operator+(const Vector3 &vec2) const {
    return {this->x + vec2.x, this->y + vec2.y, this->z + vec2.z};
}

Vector3 Vector3::operator-(const Vector3 &vec2) const {
    return {this->x - vec2.x, this->y - vec2.y, this->z - vec2.z};
}

Vector3 Vector3::operator*(const float other) const {
    return {this->x * other, this->y * other, this->z * other};
}

Vector3 Vector3::operator*(const Vector3 &other) const {
    return {this->x * other.x, this->y * other.y, this->z * other.z};
}

Vector3& Vector3::operator+=(const Vector3& other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    this->magnitude = -1.0f;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    this->magnitude = -1.0f;
    return *this;
}

const Vector3& Vector3::normalize() {
    // calculate length
    if (magnitude == -1.0f) {
        magnitude = sqrt(x * x + y * y + z * z);
    }
    // if this is a zero or normalized vector; prevent the vector having x, y, z = NaN or doing unnecessary calculations
    if (magnitude == 0.0f || magnitude == 1.0f) return *this;
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
    return *this;
}

float Vector3::dot(const Vector3 &other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3 &other) const {
    return {y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x};
}
