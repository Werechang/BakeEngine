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

Vector3& Vector3::operator+=(const Vector3 other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    this->magnitude = -1.0f;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3 other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    this->magnitude = -1.0f;
    return *this;
}

Vector3 Vector3::normalize(Vector3& vec) {
    // calculate length
    if (vec.magnitude == -1.0f) {
        vec.magnitude = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }
    // if this is a zero or normalized vector; prevent the vector having x, y, z = NaN or doing unnecessary calculations
    if (vec.magnitude == 0.0f || vec.magnitude == 1.0f) return {vec.x, vec.y, vec.z};

    return {vec.x / vec.magnitude, vec.y / vec.magnitude, vec.z / vec.magnitude};
}

float Vector3::dot(Vector3 &vec1, Vector3 &vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 Vector3::cross(Vector3 &vec1, Vector3 &vec2) {
    return {vec1.y * vec2.z - vec1.z * vec2.y,
            vec1.z * vec2.x - vec1.x * vec2.z,
            vec1.x * vec2.y - vec1.y * vec2.x};
}
