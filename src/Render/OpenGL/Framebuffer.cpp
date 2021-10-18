
#include "Framebuffer.h"
#include "../../Util/LogHelperBE.h"

Framebuffer::Framebuffer(int width, int height, int samples, Renderbuffer* renderbuffer) : width(width), height(height), samples(samples) {
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // Attach texture
    // TODO expand GLTexture class
    glGenTextures(1, &texture);
    if (samples > 1) {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);
    } else {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    }

    if (renderbuffer) {
        renderbuffer->attach();
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LogHelperBE::pushName("Framebuffer");
        LogHelperBE::error("FrameBuffer is not complete!");
        LogHelperBE::popName();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteTextures(1, &texture);
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

void Framebuffer::drawTo(const Framebuffer& fb) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.frameBuffer);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Framebuffer::resize(int nWidth, int nHeight) {
    this->width = nWidth;
    this->height = nHeight;
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    if (samples != 1) {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
    } else {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }
}
