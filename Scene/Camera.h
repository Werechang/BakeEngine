
#ifndef BAKEENGINE_CAMERA_H
#define BAKEENGINE_CAMERA_H

#include "../Util/Math/Matrix4.h"

class Camera {
private:
    Vector3 pos = Vector3(1, 1, 1);
    Vector3 target = Vector3(0, 0, 0);
    Vector3 direction = Vector3(1, 1, 1);

    Vector3 camUp = Vector3(1, 1, 1);
    Vector3 camRight = Vector3(1, 1, 1);
public:
    void translate(float distanceX, float distanceY, float distanceZ);
    void translateX(float distance);
    void translateY(float distance);
    void translateZ(float distance);
    void rotate(float angleX, float angleY, float angleZ);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    Matrix4 getView();
};


#endif
