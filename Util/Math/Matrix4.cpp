
#include "Matrix4.h"

float *Matrix4::ptr(int i, int j) {
    return &matrix[i][j];
}

Matrix4::Matrix4() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix[i][j] = i == j ? 1 : 0;
        }
    }
}

/*Matrix4& Matrix4::operator*(Matrix4 &other) {
    return ;
}*/