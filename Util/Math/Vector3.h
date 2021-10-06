// Zwischendurch einen Moment innehalten, schweigen, nachdenken, Pause machen.
#ifndef BAKEENGINE_VECTOR3_H
#define BAKEENGINE_VECTOR3_H


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
    Vector3& operator+=(Vector3 other);
    Vector3& operator-=(Vector3 other);
    static Vector3 normalize(Vector3& vec);
    static float dot(Vector3 &vec1, Vector3 &vec2);
    static Vector3 cross(Vector3& vec1, Vector3& vec2);
};


#endif
