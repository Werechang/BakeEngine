// Man bekommt beim Schweigen ganz gut ein Maß für die Zeit.
#ifndef BAKEENGINE_MATRIX4_H
#define BAKEENGINE_MATRIX4_H

#include "Vector3.h"

class Matrix4 {
private:
    float matrix[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
public:
    explicit Matrix4(float matrix2[4][4]);
    Matrix4() = default;

    static Matrix4 identity();
    static Matrix4 perspective(float fovY, float aspect, float zNear, float zFar);
    static Matrix4 orthographic(float left, float right, float bottom, float top, float zNear, float zFar);

    float* ptr(int i, int j);
    void set(int col, int row, float value);

    void multiply(float matrix2[4][4]);

    void rotateX(float rad);
    void rotateY(float rad);
    void rotateZ(float rad);
    void rotate(float xRad, float yRad, float zRad);
    void translate(float x, float y, float z);
    void translate(Vector3& vec);
    void translateX(float distance);
    void translateY(float distance);
    void translateZ(float distance);
    void scale(float xScale, float yScale, float zScale);

    Matrix4 operator*(Matrix4 &other);

    void print();
};


#endif
