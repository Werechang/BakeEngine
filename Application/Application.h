// Wir haben so vieles geschafft - wir schaffen das!
#ifndef BAKEENGINE_APPLICATION_H
#define BAKEENGINE_APPLICATION_H

#include <thread>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../Input/InputManager.h"
#include "../Scene/Camera.h"

class Application {
private:
    int width, height;
    const char* name;
    bool isOGL;

    Camera camera = Camera();

    InputManager inputManager = InputManager();
    GLFWwindow *window{};

    std::jthread renderThread;
public:
    bool running = true;

    explicit Application(bool isOGL, int width, int height, const char *name);

    void init();
    void start();
    void runGL();
    void runVk();
    void terminate();

    void getInput();
};


#endif
