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

/**
 * Generates an identity matrix. If multiplied with Matrix M1 results in M1 (M1 * identity = M1)
 * @return Identity Matrix
 */
Matrix4 Matrix4::identity() {
    return {};
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

    Matrix4 mat;
    mat[0][0] = 1/(aspect * fovTanHalf);
    mat[1][1] = 1/(fovTanHalf);
    mat[2][2] = -(zFar + zNear)/(zFar - zNear);
    mat[2][3] = -1;
    mat[3][2] = -(2*zFar*zNear)/(zFar - zNear);
    mat[3][3] = 0;
    return mat;
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
    Matrix4 mat;
    mat[0][0] = 2/(right - left);
    mat[1][1] = 2/(top - bottom);
    mat[2][2] = -2/(zFar - zNear);
    mat[3][0] = -(right + left)/(right - left);
    mat[3][1] = -(top + bottom)/(top - bottom);
    mat[3][2] = -(zFar+zNear)/(zFar - zNear);
    return mat;
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


void Matrix4::translate(const Vector3& vec) {
    matrix[3][0] += vec.x;
    matrix[3][1] += vec.y;
    matrix[3][2] += vec.z;
}

void Matrix4::scale(float xScale, float yScale, float zScale) {
    matrix[0][0] *= xScale;
    matrix[1][1] *= yScale;
    matrix[2][2] *= zScale;
}

Matrix4 Matrix4::operator*(const Matrix4 &other) const {
    Matrix4 ret;
    for (auto i = 0; i < 4; i++) {
        for (auto j = 0; j < 4; j++) {
            ret.matrix[j][i] = this->matrix[0][i] * other.matrix[j][0] + this->matrix[1][i] * other.matrix[j][1] + this->matrix[2][i] * other.matrix[j][2] + this->matrix[3][i] * other.matrix[j][3];
        }
    }
    return ret;
}

void Matrix4::print() const {
    for (auto i = 0; i<4;i++) {
        std::cout << "|" << matrix[0][i] << " " << matrix[1][i] << " " << matrix[2][i] << " " << matrix[3][i] << "|" << std::endl;
    }
}

float* Matrix4::operator[](const uint32_t index) const {
    return (float*)matrix[index];
}
