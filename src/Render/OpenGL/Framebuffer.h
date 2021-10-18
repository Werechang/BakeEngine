
#pragma once

#include "glad/gl.h"
#include "Renderbuffer.h"

class Framebuffer {
public:
    Framebuffer(int width, int height, int samples, Renderbuffer* renderbuffer);
    ~Framebuffer();

    void bind() const;
    void unbind() const;
    void bindTexture() const;
    void drawTo(const Framebuffer& fb) const;
    void resize(int nWidth, int nHeight);
private:
    uint32_t frameBuffer;
    uint32_t texture;
    int width, height;
    int samples;
};
