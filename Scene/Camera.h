
#ifndef BAKEENGINE_CAMERA_H
#define BAKEENGINE_CAMERA_H

#include "../Util/Math/Matrix4.h"
#include <cmath>

class Camera {
private:

    float yaw = -90, pitch = 0;

public:
    Vector3 pos;
    Vector3 up = Vector3(0, 1, 0);
    Vector3 front = Vector3(0, 0, -1);
    Vector3 camUp = Vector3(0, 1, 0);

    float speed = 0.05;
    float globalSpeed = 2.5f;

    Camera();
    Camera(Vector3& pos, Vector3& up);

    void moveFront();
    void moveBack();
    void moveLeft();
    void moveRight();
    void turn(float xOffset, float yOffset);

    Matrix4 getView();
};


#endif
