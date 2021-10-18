
#include "Camera.h"
#include "../Util/Math/Math.h"

Camera::Camera() : pos(0, 0, 0), up(0, 1, 0) {

}

Camera::Camera(const Vector3 &pos, const Vector3 &up) : pos(pos), up(up) {

}

void Camera::moveFront() {
    pos += front * speed;
}

void Camera::moveBack() {
    pos -= front * speed;
}

void Camera::moveLeft() {
    pos -= front.cross(camUp).normalize() * speed;
}

void Camera::moveRight() {
    pos += front.cross(camUp).normalize() * speed;
}

void Camera::moveUp() {
    pos += camUp * speed;
}

void Camera::moveDown() {
    pos -= camUp * speed;
}

void Camera::turn(float xOffset, float yOffset) {
    yaw -= xOffset;
    pitch += yOffset;
    clamp(-89.0f, 89.0f, pitch);

    front = Vector3(cos(toRadians(yaw)) * cos(toRadians(pitch)), sin(toRadians(pitch)), sin(toRadians(yaw)) * cos(toRadians(pitch)));
    front.normalize();
}

Matrix4 Camera::getView() {
    Vector3 camRight = front.cross(up);
    camUp = camRight.cross(front);
    Matrix4 view;
    view[0][0] = camRight.x;
    view[0][1] = camUp.x;
    view[0][2] = -front.x;
    view[1][0] = camRight.y;
    view[1][1] = camUp.y;
    view[1][2] = -front.y;
    view[2][0] = camRight.z;
    view[2][1] = camUp.z;
    view[2][2] = -front.z;
    view[3][0] = -camRight.dot(pos);
    view[3][1] = -camUp.dot(pos);
    view[3][2] = front.dot(pos);
    return view;
}

const Vector3 &Camera::getPos() const {
    return pos;
}
