
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const float* vertices, uint32_t size) {
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

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
