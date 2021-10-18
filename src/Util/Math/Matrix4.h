// Man bekommt beim Schweigen ganz gut ein Maß für die Zeit.
#pragma once

#include <cstdint>
#include "Vector3.h"

class Matrix4 {
private:
    float matrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
public:
    Matrix4() = default;

    static Matrix4 identity();
    static Matrix4 perspective(float fovY, float aspect, float zNear, float zFar);
    static Matrix4 orthographic(float left, float right, float bottom, float top, float zNear, float zFar);

    void multiply(float matrix2[4][4]);

    void rotateX(float rad);
    void rotateY(float rad);
    void rotateZ(float rad);
    void rotate(float xRad, float yRad, float zRad);
    void translate(float x, float y, float z);
    void translate(const Vector3& vec);
    void translateX(float distance);
    void translateY(float distance);
    void translateZ(float distance);
    void scale(float xScale, float yScale, float zScale);

    Matrix4 operator*(const Matrix4 &other) const;

    float* operator[](uint32_t index) const;

    void print() const;
};
