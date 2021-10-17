// Now the door to a new light is opening. That's exciting and inspiring.
#pragma once

#define INPUT_ON_PRESS 1
#define INPUT_ON_ACTIVE 2
#define INPUT_ON_RELEASE 0

#include <map>
#include <vector>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "KeyBind.h"
#include "../UI/GuiElement.h"


class InputManager {
public:
    void addKeyBind(const std::function<void()>& function, int key, int mods, int onAction);

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
    bool mouseButtons[8];
    double mouseX, mouseY;
    double lastMouseX = 800, lastMouseY = 450;
    bool firstMouse = true;
    std::unordered_map<int, std::unique_ptr<KeyBind>> keyBindsOnPress;
    std::unordered_map<int, std::unique_ptr<KeyBind>> keyBindsOnActive;
    std::unordered_map<int, std::unique_ptr<KeyBind>> keyBindsOnRelease;
};
