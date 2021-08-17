// Ich denke an dichte Fenster! Kein anderes Land kann so dichte und schöne Fenster bauen.
#ifndef BAKEENGINE_GLRENDERER_H
#define BAKEENGINE_GLRENDERER_H

#include "GLObject.h"

class GLRenderer {
private:
    std::vector<GLObject> models;
public:
    void addModel(const char* shaderPath, const float* vertices, unsigned int vertSize, const unsigned int* elements, unsigned int elementsSize);
    void draw(Matrix4& projView);
};


#endif
