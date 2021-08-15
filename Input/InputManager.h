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
    Function callFun;
    int numKeys;
    Action(int id, int keys[], Function callFun, int numKeys) : id(id), callFun(callFun), numKeys(numKeys) {};
};

class InputManager {
private:
    std::list<Action> actions;
    int idPtr;
public:
    static void update();

    void addAction(int keys[], Function callFun, int numKeys);
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
