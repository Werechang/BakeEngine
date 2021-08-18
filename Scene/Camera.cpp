
#include "Camera.h"

void Camera::translate(float distanceX, float distanceY, float distanceZ) {
    pos.x += distanceX;
    pos.y += distanceY;
    pos.z += distanceZ;
}

void Camera::translateX(float distance) {
    pos.x += distance;
}

void Camera::translateY(float distance) {
    pos.y += distance;
}

void Camera::translateZ(float distance) {
    pos.z += distance;
}

void Camera::rotate(float angleX, float angleY, float angleZ) {

}

void Camera::rotateX(float angle) {

}

void Camera::rotateY(float angle) {

}

void Camera::rotateZ(float angle) {

}

Matrix4 Camera::getView() {
    float view[4][4] = {{camRight.x, camUp.x, direction.x, 0},
                        {camRight.y, camUp.y, direction.y, 0},
                        {camRight.z, camUp.z, direction.z, 0},
                        {0, 0, 0, 1}};
    float mat[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {-pos.x, -pos.y, -pos.z, 1}};
    auto v = Matrix4(view);
    auto t = Matrix4(mat);
    Matrix4 m = v * t;
    return m;
}
