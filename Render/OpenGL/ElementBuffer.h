
#ifndef BAKEENGINE_ELEMENTBUFFER_H
#define BAKEENGINE_ELEMENTBUFFER_H

#include <vector>

class ElementBuffer {
private:
    unsigned int elementBufferObject{};
    unsigned int size;
public:
    explicit ElementBuffer(const unsigned int* elements, unsigned int size);
    ~ElementBuffer();
    void bind() const;
    void unbind();
    unsigned int getSize();
};


#endif
