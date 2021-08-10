
#ifndef BAKEENGINE_MATRIX4_H
#define BAKEENGINE_MATRIX4_H

class Matrix4 {
private:
    float matrix[4][4]{};
public:
    float* ptr(int i, int j);
    //Matrix4& operator*(Matrix4 &other);
    Matrix4();
};


#endif
