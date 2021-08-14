
#include "Matrix4.h"
#include <cmath>

/*
 * matrix = {{1, 0, 0, 5}, {2, 0, 0, 6}, {3, 0, 0, 7}, {4, 0, 0, 8}}
 * matrix (real formatting):
 * 1 | 2 | 3 | 4
 * 0 | 0 | 0 | 0
 * 0 | 0 | 0 | 0
 * 5 | 6 | 7 | 8
 */

Matrix4::Matrix4(float matrix2[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->matrix[i][j] = matrix2[i][j];
        }
    }
}

/**
 * Generates a normal identity matrix
 * @return Identity Matrix
 */
Matrix4 Matrix4::identity() {
    return {};
}
/**
 *
 * @param fovY Field of view on the y-axis
 * @param aspect aspect ratio (width/height)
 * @param zNear nearest clip
 * @param zFar farthest clip
 * @return Perspective Matrix4
 */
Matrix4 Matrix4::perspective(float fovY, float aspect, float zNear, float zFar) {
    float fovTanHalfAspect = tan(fovY/2) * aspect;
    // top = fovTanHalf * zNear
    // bottom = -top, right = top * aspect, left = -top * aspect

    float mat[4][4] = {{1/(fovTanHalfAspect), 0, 0, 0},
                       {0, 1/(fovTanHalfAspect), 0, 0},
                       {0, 0, -(zFar + zNear)/(zFar - zNear), -1},
                       {0, 0, -(2*zFar*zNear)/(zFar-zNear), 0}};
    return Matrix4(mat);
}

float *Matrix4::ptr(int i, int j) {
    return &matrix[i][j];
}

void Matrix4::set(int col, int row, float value) {
    matrix[col][row] = value;
}

void Matrix4::multiply(float matrix2[4][4]) {
    for (auto & i : matrix) {
        float bufferA = i[0];
        float bufferB = i[1];
        float bufferC = i[2];
        float bufferD = i[3];
        for (int j = 0; j < 4; j++) {
            i[j] = bufferA*matrix2[0][j] + bufferB*matrix2[1][j] + bufferC*matrix2[2][j] + bufferD*matrix2[3][j];
        }
    }
}

void Matrix4::rotateX(float rad) {
    float mRot[4][4] = {{1, 0, 0, 0}, {0, cos(rad), sin(rad), 0}, {0, -sin(rad), cos(rad), 0}, {0, 0, 0, 1}};
    multiply(mRot);
}

void Matrix4::rotateY(float rad) {
    float mRot[4][4] = {{cos(rad), 0, -sin(rad), 0}, {0, 1, 0, 0}, {sin(rad), 0, cos(rad), 0}, {0, 0, 0, 1}};
    multiply(mRot);
}

void Matrix4::rotateZ(float rad) {
    float mRot[4][4] = {{cos(rad), sin(rad), 0, 0}, {-sin(rad), cos(rad), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    multiply(mRot);
}

void Matrix4::rotate(float xRad, float yRad, float zRad) {
    rotateX(xRad);
    rotateY(yRad);
    rotateZ(zRad);
}

void Matrix4::translate(float x, float y, float z) {
    matrix[3][0] += x;
    matrix[3][1] += y;
    matrix[3][2] += z;
}

void Matrix4::scale(float xScale, float yScale, float zScale) {
    matrix[0][0] *= xScale;
    matrix[1][1] *= yScale;
    matrix[2][2] *= zScale;
}

Matrix4 Matrix4::operator*(Matrix4 &other) {
    float mBuf[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mBuf[i][j] = this->matrix[i][0] * other.matrix[0][j] + this->matrix[i][1] * other.matrix[1][j] + this->matrix[i][2] * other.matrix[2][j] + this->matrix[i][3] * other.matrix[3][j];
        }
    }

    return Matrix4(mBuf);
}