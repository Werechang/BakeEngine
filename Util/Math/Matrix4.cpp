
#include "Matrix4.h"
#include <cmath>

float *Matrix4::ptr(int i, int j) {
    return &matrix[i][j];
}

void Matrix4::multiply(float** matrix2) {
    for (auto & i : matrix) {
        for (int j = 0; j < 4; j++) {
            i[j] = i[0]*matrix2[0][j] + i[1]*matrix2[1][j] + i[2]*matrix2[2][j] + i[3]*matrix2[3][j];
        }
    }
}

void Matrix4::rotateX(float degrees) {
    float mRot[4][4] = {{1, 0, 0, 0}, {0, cos(degrees), -sin(degrees), 0}, {0, sin(degrees), cos(degrees), 0}, {0, 0, 0, 1}};
    multiply((float**)mRot);
}

void Matrix4::rotateY(float degrees) {
    float mRot[4][4] = {{cos(degrees), 0, sin(degrees), 0}, {0, 1, 0, 0}, {-sin(degrees), 0, cos(degrees), 0}, {0, 0, 0, 1}};
    multiply((float**)mRot);
}

void Matrix4::rotateZ(float degrees) {
    float mRot[4][4] = {{cos(degrees), -sin(degrees), 0, 0}, {sin(degrees), cos(degrees), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    multiply((float**)mRot);
}

void Matrix4::rotate(float xDegrees, float yDegrees, float zDegrees) {
    rotateX(xDegrees);
    rotateY(yDegrees);
    rotateZ(zDegrees);
}

void Matrix4::translate(float x, float y, float z) {
    float mTrans[4][4] = {{1, 0, 0, x}, {0, 1, 0, y}, {0, 0, 1, z}, {0, 0, 0, 1}};
    multiply((float **)mTrans);
}

void Matrix4::scale(float xScale, float yScale, float zScale) {
    float mScale[4][4] = {{xScale, 0, 0, 0}, {0, yScale, 0, 0}, {0, 0, zScale, 0}, {0, 0, 0, 1}};
    multiply((float **)mScale);
}

Matrix4::Matrix4(float **matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->matrix[i][j] = matrix[i][j];
        }
    }
}

Matrix4& Matrix4::operator*(Matrix4 &other) {
    for (auto & i : matrix) {
        for (int j = 0; j < 4; j++) {
            i[j] = i[0]*other.matrix[0][j] + i[1]*other.matrix[1][j] + i[2]*other.matrix[2][j] + i[3]*other.matrix[3][j];
        }
    }
    return *this;
}