
#include "Framebuffer.h"
#include "../../Util/LogHelperBE.h"

Framebuffer::Framebuffer(int width, int height, unsigned short samples, bool hasRenderBuffer) : width(width), height(height), samples(samples) {
    LogHelperBE::pushName("Framebuffer");
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // Attach texture
    glGenTextures(1, &texture);

    if (samples != 1) {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
    } else {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, samples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, texture, 0);

    // Attach depth and stencil buffer
    if (hasRenderBuffer) {
        glGenRenderbuffers(1, &renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
        if (samples != 1) {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
        } else {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        }
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LogHelperBE::error("FrameBuffer is not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    LogHelperBE::popName();
}

Framebuffer::~Framebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &renderBuffer);
    glDeleteFramebuffers(1, &frameBuffer);
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void Framebuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bindTexture() const {
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Framebuffer::drawTo(Framebuffer* fb) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->frameBuffer);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
