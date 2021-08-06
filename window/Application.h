// Wir haben so vieles geschafft - wir schaffen das!
#ifndef BAKEENGINE_APPLICATION_H
#define BAKEENGINE_APPLICATION_H

#include <iostream>
#include "OpenGLApp.h"

class Application {
private:
    bool running = true;
    bool isOGL;
    AbstractApplication *app;
public:
    explicit Application(bool isOGL, int width, int height, const char *name);
};


#endif
