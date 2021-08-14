// Now the door to a new light is opening. That's exciting and inspiring.
#ifndef BAKEENGINE_INPUTMANAGER_H
#define BAKEENGINE_INPUTMANAGER_H

#include <functional>
#include <map>
#include "GLFW/glfw3.h"

typedef void (*Function)();

class Action {
public:
    int id;
    int* keys;
    Function callFun;
    Action(int id, int* keys, Function callFun) : id(id), keys(keys), callFun(callFun) {};
};

class InputManager {
private:
    std::list<Action> actions;
    int idPtr;
public:
    static void update();

    void addAction(int *keys, Function callFun);
};

static bool KeyArray[349];

class Keys {
public:
    static void update(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_UNKNOWN) return;
        if (action == GLFW_PRESS) KeyArray[key] = true;
        if (action == GLFW_RELEASE) KeyArray[key] = false;
    }
};


#endif
