// Wir haben so vieles geschafft - wir schaffen das!
#ifndef BAKEENGINE_APPLICATION_H
#define BAKEENGINE_APPLICATION_H

#include <iostream>
#include "OpenGLApp.h"

class Application {
private:
    AbstractApplication *app = nullptr;
public:
    bool running = true;

    explicit Application(bool isOGL, int width, int height, const char *name);
};


#endif
