
#include "Camera.h"
#include "../Util/Math/Math.h"

Camera::Camera() : pos(0, 0, 0), up(0, 1, 0) {

}

Camera::Camera(Vector3 &pos, Vector3 &up) : pos(pos), up(up) {

}

void Camera::setPos(float x, float y, float z) {
    pos.x = x;
    pos.y = y;
    pos.z = z;
}

float Camera::posX() const {
    return pos.x;
}

float Camera::posY() const {
    return pos.y;
}

float Camera::posZ() const {
    return pos.z;
}

void Camera::moveFront() {
    pos += front * speed;
}

void Camera::moveBack() {
    pos -= front * speed;
}

void Camera::moveLeft() {
    pos += Vector3::normalize(Vector3::cross(front, camUp)) * speed;
}

void Camera::moveRight() {
    pos -= Vector3::normalize(Vector3::cross(front, camUp)) * speed;
}

void Camera::turn(float xOffset, float yOffset) {
    yaw -= xOffset;
    pitch += yOffset;
    Math::clamp(-89.0f, 89.0f, pitch);

    float x = cos(Math::toRadians(yaw)) * cos(Math::toRadians(pitch));
    float y = sin(Math::toRadians(pitch));
    float z = sin(Math::toRadians(yaw)) * cos(Math::toRadians(pitch));
    front = Vector3::normalize(Vector3(x, y, z));
}

Matrix4 Camera::getView() {
    Vector3 direction = Vector3::normalize(front);
    Vector3 camRight(Vector3::normalize(Vector3::cross(up, direction)));
    camUp = Vector3::cross(direction, camRight);

    float view[4][4] = {{camRight.x, camUp.x, -direction.x, 0},
                        {camRight.y, camUp.y, -direction.y, 0},
                        {camRight.z, camUp.z, -direction.z, 0},
                        {-Vector3::dot(camRight, pos), -Vector3::dot(camUp, pos), Vector3::dot(direction, pos), 1}};
    return Matrix4(view);
}
