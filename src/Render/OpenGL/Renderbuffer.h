
#pragma once

#include <cstdint>
#include "glad/gl.h"


class Renderbuffer {
public:
    Renderbuffer(int width, int height, int samples, int type, int attachmentType);
    ~Renderbuffer();
    void attach() const;
    void resize(int newWidth, int newHeight) const;
private:
    uint32_t renderBuffer;
    int samples;
    int type;
    int attachmentType;
};


