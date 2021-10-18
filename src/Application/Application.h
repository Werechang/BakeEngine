// Wir haben so vieles geschafft - wir schaffen das!
#pragma once

#define GLFW_INCLUDE_NONE

#include <thread>
#include <filesystem>
#include "../Render/OpenGL/GLRenderer.h"
#include "../Input/InputManager.h"
#include "../Scene/Camera.h"
#include "../Util/LogHelperBE.h"

static bool running = true;
static InputManager* inputManagerPtr;

class Application {
public:
    static std::string absolutePath;

    Application(bool isOGL, int width, int height, const std::string& name);
    ~Application() {
        LogHelperBE::popName();
    }

    void init();
    void start();
    void runGL();
    void runVk();
    void terminate();
private:
    int width, height;
    const std::string& name;
    bool isOGL;

    Camera camera = Camera();
    GLFWwindow *window{};
};
