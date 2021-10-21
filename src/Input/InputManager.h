// Now the door to a new light is opening. That's exciting and inspiring.
#pragma once

#define INPUT_ON_PRESS 1
#define INPUT_ON_ACTIVE 2
#define INPUT_ON_RELEASE 0
#define GLFW_MOUSE_BUTTONS 349

#include <map>
#include <vector>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "KeyBind.h"
#include "../UI/GuiElement.h"


class InputManager {
public:
    void addKeyBind(const std::function<void()>& function, int key, int mods, int action, GuiElement* onHoverElement);

    // Called every frame by the application
    void updateInput(GLFWwindow* window);

    void updateKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void updateMouse(GLFWwindow *window, double xPos, double yPos);
    void updateMouseButton(GLFWwindow* window, int button, int action, int mods);

    static const char* getKeyName(int key);
    static const char* getActionName(int action);
    static const char* getButtonName(int button);
private:
    // For mouse repeat. Maybe deleted later
    int activeMods;
    std::map<int, bool> activeKeys;
    double mouseX, mouseY;
    double lastMouseX = 800, lastMouseY = 450;
    bool firstMouse = true;
    // map<pair<key, action>, KeyBind>, unordered map doesn't work with pairs
    std::map<std::pair<int, int>, std::unique_ptr<KeyBind>> keyBinds;
    // The element over which the mouse is currently hovering
    GuiElement* currentHoverElement;
    // The GuiElement which was left clicked last
    GuiElement* selectedElement;
};
