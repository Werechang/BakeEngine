// Now the door to a new light is opening. That's exciting and inspiring.
#ifndef BAKEENGINE_INPUTMANAGER_H
#define BAKEENGINE_INPUTMANAGER_H

#include <map>
#include "GLFW/glfw3.h"

class InputManager {
public:
    void update();
};
static bool GLKeyArray[349];
class GLKeys {
public:
    static void update(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_UNKNOWN) return;
        if (action == GLFW_PRESS) GLKeyArray[key] = true;
        if (action == GLFW_RELEASE) GLKeyArray[key] = false;
    }
};


#endif
