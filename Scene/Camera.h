
#ifndef BAKEENGINE_CAMERA_H
#define BAKEENGINE_CAMERA_H

#include "../Util/Math/Matrix4.h"
#include <cmath>

class Camera {
private:
    Vector3 pos;
    Vector3 up = Vector3(0, 1, 0);
    Vector3 front = Vector3(0, 0, -1);
    Vector3 camUp = Vector3(0, 1, 0);

    float yaw = -90, pitch = 0;

public:
    float speed = 0.05;

    Camera();
    Camera(Vector3& pos, Vector3& up);

    void setPos(float x, float y, float z);

    float posX() const;
    float posY() const;
    float posZ() const;

    void moveFront();
    void moveBack();
    void moveLeft();
    void moveRight();
    void turn(float xOffset, float yOffset);

    Matrix4 getView();
};


#endif
