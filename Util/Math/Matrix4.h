
#ifndef BAKEENGINE_MATRIX4_H
#define BAKEENGINE_MATRIX4_H

class Matrix4 {
private:
    float matrix[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
public:
    float* ptr(int i, int j);
    Matrix4& operator*(Matrix4 &other);
    explicit Matrix4(float** matrix);
    Matrix4() = default;

    void multiply(float** matrix2);

    void rotateX(float degrees);
    void rotateY(float degrees);
    void rotateZ(float degrees);
    void rotate(float xDegrees, float yDegrees, float zDegrees);
    void translate(float x, float y, float z);
    void scale(float xScale, float yScale, float zScale);
};


#endif
