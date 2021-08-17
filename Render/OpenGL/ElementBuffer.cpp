
#include "ElementBuffer.h"
#include "glad/glad.h"

ElementBuffer::ElementBuffer(const unsigned int* elements, unsigned int size) : size(size){
    glGenBuffers(1, &elementBufferObject);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, elements, GL_STATIC_DRAW);
}

ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, &elementBufferObject);
}

void ElementBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, elementBufferObject);
}

void ElementBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int ElementBuffer::getSize() {
    return size;
}
