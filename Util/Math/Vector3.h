// Zwischendurch einen Moment innehalten, schweigen, nachdenken, Pause machen.
#ifndef BAKEENGINE_VECTOR3_H
#define BAKEENGINE_VECTOR3_H


class Vector3 {
private:
    float x, y, z;
public:
    Vector3(float x, float y, float z);
    float* get4();
    float getX() const;
    float getY() const;
    float getZ() const;

    Vector3 operator+(Vector3& other) const;
};


#endif
