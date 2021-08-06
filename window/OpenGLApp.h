// Wir schaffen das, und dort, wo uns etwas im Wege steht, muss es überwunden werden.
#ifndef BAKEENGINE_OPENGLAPP_H
#define BAKEENGINE_OPENGLAPP_H

#include <thread>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "AbstractApplication.h"

class OpenGLApp : public AbstractApplication {
private:
    int width, height;
    GLFWwindow *window;
public:
    OpenGLApp(int width, int height, const char *name);

    void run() override;
    void terminate() override;
};


#endif
