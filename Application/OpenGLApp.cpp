// Wir schaffen das, und dort, wo uns etwas im Wege steht, muss es überwunden werden.
#include <iostream>
#include "OpenGLApp.h"

void errorCallback(int error, const char *description) {
    std::cerr << "GL Error (" << error << "): " << description << std::endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}

OpenGLApp::OpenGLApp(int width, int height, const char *name, bool *running) : width(width), height(height), running(running) {
    glfwInit();

    // Error function to print out errors
    glfwSetErrorCallback(errorCallback);

    // Specify OGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core for easier coding; newer devices only
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creates Application
    window = glfwCreateWindow(width, height, name, nullptr, nullptr);

    // exit when Application was not initialized properly
    if (window == nullptr) {
        std::cerr << "Error while creating Application context" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    // usually 0 but 1 is better for syncing reasons. On fast computers, buffers may be swapped
    // in the middle of a frame, leading to tearing.
    glfwSwapInterval(1);

    // Function for inputs
    glfwSetKeyCallback(window, keyCallback);

    gladLoadGL();
}

void OpenGLApp::run() {
    while (!glfwWindowShouldClose(window)) {

        glfwGetFramebufferSize(window, &width, &height);
        // glfwSetFrameBufferSizeCallback for future use
        glViewport(0, 0, width, height);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}

void OpenGLApp::terminate() {
    running = *false;
    glfwDestroyWindow(window);
    glfwTerminate();
}