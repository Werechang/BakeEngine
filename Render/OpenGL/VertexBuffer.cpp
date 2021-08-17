
#include "VertexBuffer.h"
#include "glad/glad.h"

VertexBuffer::VertexBuffer(const float* vertices, unsigned int size) {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &vertexBuffer);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
}

void VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
