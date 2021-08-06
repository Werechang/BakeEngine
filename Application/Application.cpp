// Wir haben so vieles geschafft - wir schaffen das!
#include "Application.h"
#include <thread>

Application::Application(bool isOGL, int width, int height, const char *name) {
    if (isOGL) {
        std::cout << "Starting OpenGL Application ..." << std::endl;
        OpenGLApp oglApp = OpenGLApp(width, height, name);
        app = &oglApp;
    } else {
        std::cout << "Starting Vulkan Application ..." << std::endl;
    }
    if (app == nullptr) {
        std::cerr << "Error while initializing " << (isOGL ? "OpenGL" : "Vulkan") << " application" << std::endl;
        exit(-1);
    }
    std::jthread renderThread = std::jthread([] {
        app->run();
        app->terminate();
    })
    while (running) {

    }
}