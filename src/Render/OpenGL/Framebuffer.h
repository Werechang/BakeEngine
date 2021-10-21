
#pragma once

#include <memory>
#include "glad/gl.h"
#include "Renderbuffer.h"
#include "GLTexture.h"

class Framebuffer {
public:
    Framebuffer(int width, int height, int samples);
    ~Framebuffer();

    void attachRenderbuffer(int type, int attachmentType);
    void attachTexture(int type, int attachmentType);

    void bind() const;
    void unbind() const;
    void bindTexture(int slot) const;
    void drawTo(const Framebuffer& fb) const;
    void resize(int nWidth, int nHeight);

    void checkStatus() const;
private:
    std::unique_ptr<GLTexture> texture;
    std::unique_ptr<Renderbuffer> renderbuffer;
    int texAttachmentType, texType, renderbufferAttachmentType, renderbufferType;
    uint32_t framebuffer;
    int width, height;
    int samples;
};
