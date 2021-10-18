
#pragma once

#include <vector>

class ElementBuffer {
private:
    uint32_t elementBufferObject{};
    uint32_t size;
public:
    explicit ElementBuffer(const uint32_t* elements, uint32_t size);
    ~ElementBuffer();
    void bind() const;
    void unbind() const;
    uint32_t getSize() const;
};
