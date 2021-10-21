
#pragma once

#include <cstdint>
#include "glad/gl.h"


class Renderbuffer {
public:
    Renderbuffer(int width, int height, int samples, int type);
    ~Renderbuffer();

    void resize(int newWidth, int newHeight) const;

    void bind() const;
    void unbind() const;

    uint32_t get() const;
private:
    uint32_t renderbuffer;
    int samples;
    int type;
};


