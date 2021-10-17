
#pragma once

#include "../Util/Math/Matrix4.h"
#include <cmath>

class Camera {
public:

    float speed = 0.05;
    float globalSpeed = 2.5f;

    Camera();
    Camera(Vector3& pos, Vector3& up);

    void moveFront();
    void moveBack();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void turn(float xOffset, float yOffset);

    Matrix4 getView();
    const Vector3& getPos() const;
private:
    float yaw = -90, pitch = 0;
    Vector3 pos;
    Vector3 up = Vector3(0, 1, 0);
    Vector3 front = Vector3(0, 0, -1);
    Vector3 camUp = Vector3(0, 1, 0);
};
