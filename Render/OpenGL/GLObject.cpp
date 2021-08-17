// Mit dem Kopf durch die Wand wird nicht gehen. Da siegt zum Schluss immer die Wand.
#include "GLObject.h"

GLObject::~GLObject() {
}

GLObject::GLObject(const char* shaderPath, const float* vertices, unsigned int vertSize, const unsigned int* elements, unsigned int elementsSize) : shader(GLShader(shaderPath)),
                                                                                                                                                    elementBuffer(elements, elementsSize),
                                                                                                                                                    vertexArray(VertexArray()),
                                                                                                                                                    attributes(VertexAttributes()){
    // TODO: File parser (COLLADA .dae)
    vertexArray = VertexArray();
    attributes.addAttribute<float>(3);
    attributes.addAttribute<float>(3);
    attributes.addAttribute<float>(2);
    attributes.addAttribute<float>(3);

    vertexArray.addVertexBuffer(VertexBuffer(vertices, vertSize), attributes);

    matrix = Matrix4::identity();
}

void GLObject::render(Matrix4 &projView) {
    shader.bind();
    vertexArray.bind();
    elementBuffer.bind();

    shader.uniformMatrix4fv("vp", projView);
    shader.uniformMatrix4fv("model", matrix);
    glDrawElements(GL_TRIANGLES, elementBuffer.getSize(), GL_UNSIGNED_INT, nullptr);
}
