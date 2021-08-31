
#ifndef BAKEENGINE_FRAMEBUFFER_H
#define BAKEENGINE_FRAMEBUFFER_H

#include "glad/glad.h"

class Framebuffer {
private:
    unsigned int frameBuffer;
    unsigned int texture;
    unsigned int renderBuffer;
    int width, height;
public:
    Framebuffer(int width, int height, unsigned short samples = 1, bool hasRenderBuffer = true);
    ~Framebuffer();

    void bind() const;
    void unbind() const;
    void bindTexture() const;
    void drawTo(Framebuffer* fb) const;
};


#endif
