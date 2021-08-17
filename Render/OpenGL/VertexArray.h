
#ifndef BAKEENGINE_VERTEXARRAY_H
#define BAKEENGINE_VERTEXARRAY_H

#include "VertexBuffer.h"

class VertexArray {
private:
    unsigned int vertexArrayObject;
public:
    VertexArray();
    ~VertexArray();
    void bind() const;
    void unbind();
    void addVertexBuffer(const VertexBuffer& buffer, const VertexAttributes& attributes) const;
};


#endif
