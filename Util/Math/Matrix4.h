
#ifndef BAKEENGINE_MATRIX4_H
#define BAKEENGINE_MATRIX4_H

#include "Vector3.h"

class Matrix4 {
private:
    float matrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
public:
    explicit Matrix4(float matrix2[4][4]);
    Matrix4() = default;

    static Matrix4 identity();
    static Matrix4 perspective(float fovY, float aspect, float zNear, float zFar);

    float* ptr(int i, int j);
    void set(int col, int row, float value);

    void multiply(float matrix2[4][4]);

    void rotateX(float rad);
    void rotateY(float rad);
    void rotateZ(float rad);
    void rotate(float xRad, float yRad, float zRad);
    void translate(float x, float y, float z);
    void scale(float xScale, float yScale, float zScale);

    Matrix4 operator*(Matrix4 &other);
};


#endif
