
#ifndef BAKEENGINE_MATRIX4_H
#define BAKEENGINE_MATRIX4_H

#include "Vector3.h"

class Matrix4 {
private:
    float matrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
public:
    float* ptr(int i, int j);
    explicit Matrix4(float matrix2[4][4]);
    Matrix4() = default;

    void multiply(float matrix2[4][4]);
    void multiply(Vector3& vector);

    void rotateX(float degrees);
    void rotateY(float degrees);
    void rotateZ(float degrees);
    void rotate(float xDegrees, float yDegrees, float zDegrees);
    void translate(float x, float y, float z);
    void scale(float xScale, float yScale, float zScale);

    Matrix4 operator*(Matrix4 &other);
    Matrix4 operator*(Vector3 &other);
};


#endif
