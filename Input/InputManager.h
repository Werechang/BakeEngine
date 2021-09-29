// Now the door to a new light is opening. That's exciting and inspiring.
#ifndef BAKEENGINE_INPUTMANAGER_H
#define BAKEENGINE_INPUTMANAGER_H

#include <map>
#include "GLFW/glfw3.h"
#include "InputNode.h"

static double mouseX, mouseY;
static double lastMouseX = 800, lastMouseY = 450;
static bool firstMouse = true;

class InputManager {
private:
    std::map<std::unique_ptr<InputNode>, unsigned int> inputNodes;
public:
    void update();
    void resortNodeTree();
    void addAction(std::function<void()>& function, int key1);
    void addAction(std::function<void()>& function, int key1, int key2);
    void addAction(std::function<void()>& function, int key1, int key2, int key3);
    void addAction(std::function<void()>& function, int key1, int key2, int key3, int key4);

    InputNode* findKeyLayer0(int key);

    static void updateKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_UNKNOWN) return;
        if (action == GLFW_PRESS) KeyboardKeys[key] = true;
        if (action == GLFW_RELEASE) KeyboardKeys[key] = false;
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

    static void updateMouseButton(GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            if (!MouseButtons[button])
                MouseButtonWasJustPressed[button] = true;
            MouseButtons[button] = true;
        }
        if (action == GLFW_RELEASE) {
            MouseButtons[button] = false;
            MouseButtonWasJustPressed[button] = false;
        }
    };
};

#endif
