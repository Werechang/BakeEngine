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

static bool KeyArray[349];
static double mouseX, mouseY;
static double lastMouseX = 800, lastMouseY = 450;
static bool firstMouse = true;

class InputManager {
private:
    std::list<Action> actions;
    int idPtr;
public:
    static void update();

    static void updateKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_UNKNOWN) return;
        if (action == GLFW_PRESS) KeyArray[key] = true;
        if (action == GLFW_RELEASE) KeyArray[key] = false;
    }

    static void updateMouse(GLFWwindow *window, double xPos, double yPos) {
        if (firstMouse) {
            lastMouseX = xPos;
            lastMouseY = yPos;
            firstMouse = false;
        }
        mouseX = xPos;
        mouseY = yPos;
    }

    void addAction(int keys[], Function callFun, int numKeys);
};

#endif
