
#include "Camera.h"
#include "../Util/Math/Math.h"

Camera::Camera() : pos(0, 0, 0), up(0, 1, 0) {

}

Camera::Camera(Vector3 &pos, Vector3 &up) : pos(pos), up(up) {

}

void Camera::moveFront() {
    pos += front * speed;
}

void Camera::moveBack() {
    pos -= front * speed;
}

void Camera::moveLeft() {
    auto left = Vector3::cross(front, camUp);
    pos += Vector3::normalize(left) * speed;
}

void Camera::moveRight() {
    auto left = Vector3::cross(front, camUp);
    pos -= Vector3::normalize(left) * speed;
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

    float x = cos(toRadians(yaw)) * cos(toRadians(pitch));
    float y = sin(toRadians(pitch));
    float z = sin(toRadians(yaw)) * cos(toRadians(pitch));
    auto vec = Vector3(x, y, z);
    front = Vector3::normalize(vec);
}

Matrix4 Camera::getView() {
    Vector3 direction = Vector3::normalize(front);
    auto vec = Vector3::cross(up, direction);
    Vector3 camRight(Vector3::normalize(vec));
    camUp = Vector3::cross(direction, camRight);

    float view[4][4] = {{camRight.x, camUp.x, -direction.x, 0},
                        {camRight.y, camUp.y, -direction.y, 0},
                        {camRight.z, camUp.z, -direction.z, 0},
                        {-Vector3::dot(camRight, pos), -Vector3::dot(camUp, pos), Vector3::dot(direction, pos), 1}};
    return Matrix4(view);
}

const Vector3 &Camera::getPos() const {
    return pos;
}
