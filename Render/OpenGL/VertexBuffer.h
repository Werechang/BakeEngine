
#ifndef BAKEENGINE_VERTEXBUFFER_H
#define BAKEENGINE_VERTEXBUFFER_H

#include <vector>
#include "glad/glad.h"

class VertexBuffer {
private:
    unsigned int vertexBuffer{};
public:
    explicit VertexBuffer(const float* vertices, unsigned int size);
    ~VertexBuffer();
    void bind() const;
    void unbind();
};

struct VertexAttribute {
    unsigned int type;
    unsigned int size;
    bool isNormalized;

    static unsigned int getSizeFromType(const unsigned int type) {
        switch (type) {
            case GL_FLOAT: return 4;
            default: return 0;
        }
    };
};

class VertexAttributes {
private:
    std::vector<VertexAttribute> attribs;
    // vertexSize = stride
    unsigned int vertexSize = 0;
public:
    template<typename T>
    void addAttribute(unsigned int count) {
    };

    template<>
    void addAttribute<float>(unsigned int count) {
        attribs.push_back({GL_FLOAT, count, GL_FALSE});
        vertexSize += count * sizeof(float);
    };

    std::vector<VertexAttribute> getAttributes() const {
        return attribs;
    };

    unsigned int getVertexSize() const {
        return vertexSize;
    };
};


#endif
