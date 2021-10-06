// Wir haben so vieles geschafft - wir schaffen das!
#ifndef BAKEENGINE_APPLICATION_H
#define BAKEENGINE_APPLICATION_H

#define GLFW_INCLUDE_NONE

#include <thread>
#include <filesystem>
#include "../Render/OpenGL/GLRenderer.h"
#include "../Input/InputManager.h"
#include "../Scene/Camera.h"
#include "../Util/LogHelperBE.h"

static bool running = true;

class Application {
private:
    int width, height;
    const char* name;
    bool isOGL;

    Camera camera = Camera();

    InputManager inputManager = InputManager();
    GLFWwindow *window{};

public:
    static std::string absolutePath;

    explicit Application(bool isOGL, int width, int height, const char *name);
    ~Application() {
        LogHelperBE::popName();
    }

    void init();
    void start();
    void runGL();
    void runVk();
    void terminate();

    void getInput(float deltaTime);
};

#endif
