
#pragma once

#include "VertexBuffer.h"

class VertexArray {
private:
    uint32_t vertexArrayObject;
public:
    VertexArray();
    ~VertexArray();
    void bind() const;
    void unbind() const;
    void addVertexBuffer(const VertexBuffer& buffer, const VertexAttributes& attributes) const;
};
