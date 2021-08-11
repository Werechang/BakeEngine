//
// Created by tom on 11.08.2021.
//

#ifndef BAKEENGINE_VECTOR3_H
#define BAKEENGINE_VECTOR3_H


class Vector3 {
private:
    float x, y, z;
public:
    Vector3(float x, float y, float z);
    float* get4();
};


#endif //BAKEENGINE_VECTOR3_H
