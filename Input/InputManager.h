// Now the door to a new light is opening. That's exciting and inspiring.
#ifndef BAKEENGINE_INPUTMANAGER_H
#define BAKEENGINE_INPUTMANAGER_H

#include <map>
#include <vector>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "InputNode.h"
#include "../UI/GuiElement.h"

static double mouseX, mouseY;
static double lastMouseX = 800, lastMouseY = 450;
static bool firstMouse = true;

class InputManager {
private:
    std::vector<InputNode> inputNodes;
    std::vector<InputNode*> inputNodesLayer0;

    bool isAnyKeyInTree(std::vector<int>& keys) const;
public:
    void update() const;
    void addAction(std::function<void()>& function, int key1);
    void addAction(std::function<void()>& function, int key1, int key2);
    void addAction(std::function<void()>& function, int key1, int key2, int key3);
    void addAction(std::function<void()>& function, int key1, int key2, int key3, int key4);

    static void updateKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_UNKNOWN) return;
        else if (action == GLFW_PRESS) {
            KeysAndButtons[key] = true;
            KeysAndButtonsPressed[key] = true;
        } else if (action == GLFW_RELEASE) {
            KeysAndButtons[key] = false;
            KeysAndButtonsPressed[key] = false;
        } else if (action == GLFW_REPEAT) {
            KeysAndButtonsPressed[key] = false;
        }
    }

    static void updateMouse(GLFWwindow *window, double xPos, double yPos) {
        if (firstMouse) {
            lastMouseX = xPos;
            lastMouseY = yPos;
            firstMouse = false;
        }
        mouseX = xPos;
        mouseY = yPos;
        int screenX = (int)std::round(mouseX);
        int screenY = (int)std::round(mouseY);
        for (auto & guiElement : GuiElement::guiElements) {
            if (guiElement->isMouseHover(screenX, screenY)) {
                break;
            }
        }
    }

    static void updateMouseButton(GLFWwindow* window, int button, int action, int mods) {
        if (action == GLFW_PRESS) {
            KeysAndButtonsPressed[349 + button] = true;
            KeysAndButtons[349 + button] = true;
        }
        else if (action == GLFW_RELEASE) {
            KeysAndButtons[349 + button] = false;
            KeysAndButtonsPressed[349 + button] = false;
        }
    };
};

#endif
