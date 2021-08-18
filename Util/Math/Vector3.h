// Zwischendurch einen Moment innehalten, schweigen, nachdenken, Pause machen.
#ifndef BAKEENGINE_VECTOR3_H
#define BAKEENGINE_VECTOR3_H


class Vector3 {
public:
    float x, y, z;
    float magnitude = 1;

    Vector3(float x, float y, float z);

    Vector3 cross(Vector3& other);
    Vector3 operator+(Vector3& other) const;
    void normalize();
};


#endif
