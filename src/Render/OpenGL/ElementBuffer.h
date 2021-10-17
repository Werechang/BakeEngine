
#pragma once

#include <vector>

class ElementBuffer {
private:
    unsigned int elementBufferObject{};
    unsigned int size;
public:
    explicit ElementBuffer(const unsigned int* elements, unsigned int size);
    ~ElementBuffer();
    void bind() const;
    void unbind() const;
    unsigned int getSize() const;
};
