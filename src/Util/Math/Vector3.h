// Zwischendurch einen Moment innehalten, schweigen, nachdenken, Pause machen.
#pragma once


class Vector3 {
private:
    // Set magnitude to -1.0f if it should be updated
    float magnitude;
public:
    float x, y, z;

    Vector3(float x, float y, float z);

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(const Vector3& other) const;
    Vector3 operator*(float other) const;
    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    const Vector3& normalize();
    float dot(const Vector3 &other) const;
    Vector3 cross(const Vector3& other) const;
};
