// Wir haben so vieles geschafft - wir schaffen das!
#include "Application.h"
#include <thread>

Application::Application(bool isOGL, int width, int height, const char *name) : isOGL(isOGL) {
    if (isOGL) {
        std::cout << "Starting with OpenGL..." << std::endl;
        OpenGLApp oglApp = OpenGLApp(width, height, name);
        app = &oglApp;
    } else {

    }
    if (app == nullptr) {
        std::cerr << "Error while initializing " << (isOGL ? "OpenGL" : "") << " application" << std::endl;
        exit(-1);
    }
    app->run();
    app->terminate();
}