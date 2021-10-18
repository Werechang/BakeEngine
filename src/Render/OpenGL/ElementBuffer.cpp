
#include "ElementBuffer.h"
#include "glad/gl.h"

ElementBuffer::ElementBuffer(const uint32_t* elements, uint32_t size) : size(size){
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

void ElementBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

uint32_t ElementBuffer::getSize() const{
    return size;
}
