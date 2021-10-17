
#pragma once

#include "glad/gl.h"

class Framebuffer {
private:
    unsigned int frameBuffer;
    unsigned int texture;
    unsigned int renderBuffer;
    int width, height;
    unsigned short samples;
public:
    Framebuffer(int width, int height, unsigned short samples = 1, bool hasRenderBuffer = true);
    ~Framebuffer();

    void bind() const;
    void unbind() const;
    void bindTexture() const;
    void drawTo(Framebuffer& fb) const;
    void resize(int nWidth, int nHeight);
};