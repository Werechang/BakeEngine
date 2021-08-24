// Zwischendurch einen Moment innehalten, schweigen, nachdenken, Pause machen.
#include "Vector3.h"
#include <cmath>

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {
}

Vector3 Vector3::operator+(Vector3 &vec2) const {
    return {this->x + vec2.x, this->y + vec2.y, this->z + vec2.z};
}

Vector3 Vector3::operator-(Vector3 &vec2) const {
    return {this->x - vec2.x, this->y - vec2.y, this->z - vec2.z};
}

Vector3 Vector3::operator*(float other) const {
    return {this->x * other, this->y * other, this->z * other};
}

Vector3 Vector3::operator*(Vector3 &other) const {
    return {this->x * other.x, this->y * other.y, this->z * other.z};
}

Vector3 Vector3::operator+=(Vector3 other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}

Vector3 Vector3::operator-=(Vector3 other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

Vector3 Vector3::normalize(const Vector3 &vec) {
    // calculate length
    float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    // if this is a zero or normalized vector; prevent the vector having x, y, z = NaN or doing unnecessary calculations
    if (magnitude == 0 || magnitude == 1) return {vec.x, vec.y, vec.z};

    return {vec.x / magnitude, vec.y / magnitude, vec.z / magnitude};
}

float Vector3::dot(Vector3 &vec1, Vector3 &vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

Vector3 Vector3::cross(Vector3 &vec1, Vector3 &vec2) {
    return {vec1.y * vec2.z - vec1.z * vec2.y,
            vec1.z * vec2.x - vec1.x * vec2.z,
            vec1.x * vec2.y - vec1.y * vec2.x};
}
