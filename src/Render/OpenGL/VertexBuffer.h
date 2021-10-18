
#pragma once

#include <vector>
#include "glad/gl.h"

class VertexBuffer {
private:
    uint32_t vertexBuffer{};
public:
    explicit VertexBuffer(const float* vertices, uint32_t size);
    ~VertexBuffer();
    void bind() const;
    void unbind() const;
};

struct VertexAttribute {
    uint32_t type;
    uint32_t size;
    bool isNormalized;

    static uint32_t getSizeFromType(uint32_t type) {
        switch (type) {
            case GL_FLOAT: return 4;
            default: return 0;
        }
    };
};

class VertexAttributes {
private:
    std::vector<VertexAttribute> attribs;
    uint32_t size = 0;
public:
    template<typename T>
    void addAttribute(uint32_t count) {
    };

    template<>
    void addAttribute<float>(uint32_t count) {
        attribs.push_back({GL_FLOAT, count, GL_FALSE});
        size += count * 4;
    };

    std::vector<VertexAttribute> getAttributes() const {
        return attribs;
    };

    uint32_t getVertexSize() const {
        return size;
    };
};
