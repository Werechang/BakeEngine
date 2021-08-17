// Mit dem Kopf durch die Wand wird nicht gehen. Da siegt zum Schluss immer die Wand.
#ifndef BAKEENGINE_GLOBJECT_H
#define BAKEENGINE_GLOBJECT_H

#include "GLShader.h"
#include "ElementBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "GLTexture.h"

class GLObject {
private:
    GLShader shader;
    Matrix4 matrix;
    ElementBuffer elementBuffer;
    VertexArray vertexArray;
    std::vector<GLTexture> textures;
    VertexAttributes attributes;
public:
    explicit GLObject(const char* shaderPath, const float* vertices, unsigned int vertSize, const unsigned int* elements, unsigned int elementsSize);
    ~GLObject();
    void render(Matrix4& projView);
};


#endif
