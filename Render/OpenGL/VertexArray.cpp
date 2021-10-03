
#include "VertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &vertexArrayObject);
}

void VertexArray::bind() const {
    glBindVertexArray(vertexArrayObject);
}

void VertexArray::unbind() const{
    glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(const VertexBuffer& buffer, const VertexAttributes& attributes) const {
    bind();
    buffer.bind();
    std::vector<VertexAttribute> attribs = attributes.getAttributes();
    unsigned int offset = 0;
    for (auto i = 0; i < attribs.size(); i++) {
        VertexAttribute attribute = attribs[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, (int)attribute.size, attribute.type, attribute.isNormalized, attributes.getVertexSize(), (void*)offset);
        offset += attribute.size * VertexAttribute::getSizeFromType(attribute.type);
    }
}
