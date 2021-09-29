// Man bekommt beim Schweigen ganz gut ein Maß für die Zeit.
#include "Matrix4.h"
#include <cmath>
#include <iostream>

/*
 * OpenGL uses column first by default
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
 * Generates an identity matrix. If multiplied with Matrix M1 results in M1 (M1 * identity = M1)
 * @return Identity Matrix
 */
Matrix4 Matrix4::identity() {
    float mat[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
    return Matrix4(mat);
}
/**
 * Generates a perspective projection matrix
 * @param fovY Field of view on the y-axis, in radians
 * @param aspect aspect ratio (width/height)
 * @param zNear near clipping plane
 * @param zFar far clipping plane
 * @return A projection (clip space) matrix
 */
Matrix4 Matrix4::perspective(float fovY, float aspect, float zNear, float zFar) {
    float fovTanHalf = tan(fovY/2);
    // bottom = -top, right = top * aspect, left = -top * aspect

    float mat[4][4] = {{1/(aspect * fovTanHalf), 0, 0, 0},
                       {0, 1/(fovTanHalf), 0, 0},
                       {0, 0, -(zFar + zNear)/(zFar - zNear), -1},
                       {0, 0, -(2*zFar*zNear)/(zFar - zNear), 0}};
    return Matrix4(mat);
}
/**
 * Generates an orthographic projection matrix
 * @param left left side of the screen, usually 0
 * @param right right side of the screen, usually width
 * @param bottom bottom of the screen, usually 0
 * @param top top of the screen, usually height
 * @param zNear near clipping plane
 * @param zFar far clipping plane
 * @return A projection (clip space) matrix
 */
Matrix4 Matrix4::orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
    float mat[4][4] = {{2/(right - left), 0, 0, 0},
                       {0, 2/(top - bottom), 0, 0},
                       {0, 0, -2/(zFar - zNear), 0},
                       {-(right + left)/(right - left), -(top + bottom)/(top - bottom), -(zFar+zNear)/(zFar - zNear), 1}};
    return Matrix4(mat);
}

/**
 *
 * @param i column 0-3
 * @param j row 0-3
 * @return A pointer to the (i,j) element of the matrix array
 */
float *Matrix4::ptr(int i, int j) {
    return &matrix[i][j];
}

void Matrix4::set(int col, int row, float value) {
    matrix[col][row] = value;
}

void Matrix4::multiply(float matrix2[4][4]) {
    for (auto i = 0; i < 4; i++) {
        float bufferA = matrix[0][i];
        float bufferB = matrix[1][i];
        float bufferC = matrix[2][i];
        float bufferD = matrix[3][i];
        for (auto j = 0; j < 4; j++) {
            matrix[j][i] = bufferA*matrix2[j][0] + bufferB*matrix2[j][1] + bufferC*matrix2[j][2] + bufferD*matrix2[j][3];
        }
    }
}

void Matrix4::rotateX(float rad) {
    float mRot[4][4] = {{1, 0, 0, 0}, {0, (float)cos(rad), (float)sin(rad), 0}, {0, (float)-sin(rad), (float)cos(rad), 0}, {0, 0, 0, 1}};
    multiply(mRot);
}

void Matrix4::rotateY(float rad) {
    float mRot[4][4] = {{(float)cos(rad), 0, (float)-sin(rad), 0}, {0, 1, 0, 0}, {(float)sin(rad), 0, (float)cos(rad), 0}, {0, 0, 0, 1}};
    multiply(mRot);
}

void Matrix4::rotateZ(float rad) {
    float mRot[4][4] = {{(float)cos(rad), (float)sin(rad), 0, 0}, {(float)-sin(rad), (float)cos(rad), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
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

void Matrix4::translateX(float distance) {
    matrix[3][0] = distance;
}

void Matrix4::translateY(float distance) {
    matrix[3][1] = distance;
}

void Matrix4::translateZ(float distance) {
    matrix[3][2] = distance;
}


void Matrix4::translate(Vector3& vec) {
    matrix[3][0] += vec.x;
    matrix[3][1] += vec.y;
    matrix[3][2] += vec.z;
}

void Matrix4::scale(float xScale, float yScale, float zScale) {
    matrix[0][0] *= xScale;
    matrix[1][1] *= yScale;
    matrix[2][2] *= zScale;
}

Matrix4 Matrix4::operator*(Matrix4 &other) {
    Matrix4 ret;
    for (auto i = 0; i < 4; i++) {
        for (auto j = 0; j < 4; j++) {
            ret.matrix[j][i] = this->matrix[0][i] * other.matrix[j][0] + this->matrix[1][i] * other.matrix[j][1] + this->matrix[2][i] * other.matrix[j][2] + this->matrix[3][i] * other.matrix[j][3];
        }
    }

    return Matrix4(ret.matrix);
}

void Matrix4::print() {
    for (auto i = 0; i<4;i++) {
        std::cout << "|" << matrix[0][i] << " " << matrix[1][i] << " " << matrix[2][i] << " " << matrix[3][i] << "|" << std::endl;
    }
}
