// Wir haben so vieles geschafft - wir schaffen das!
#ifndef BAKEENGINE_APPLICATION_H
#define BAKEENGINE_APPLICATION_H

#include <thread>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../Input/InputManager.h"
#include "../Scene/Camera.h"
#include "../Render/OpenGL/GLRenderer.h"
#include "../Util/LogHelperBE.h"

class Application {
private:
    int width, height;
    const char* name;
    bool isOGL;

    Camera camera = Camera();

    InputManager inputManager = InputManager();
    GLFWwindow *window{};

    GLRenderer renderer;
public:
    bool running = true;

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
