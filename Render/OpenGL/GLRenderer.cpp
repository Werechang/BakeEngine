// Ich denke an dichte Fenster! Kein anderes Land kann so dichte und schöne Fenster bauen.
#include "GLRenderer.h"

void GLRenderer::draw(Matrix4& projView) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (GLObject object : models) {
        object.render(projView);
    }
}

void
GLRenderer::addModel(const char *shaderPath, const float *vertices, unsigned int vertSize, const unsigned int *elements,
                     unsigned int elementsSize) {
    models.emplace_back(shaderPath, vertices, vertSize, elements, elementsSize);
}
