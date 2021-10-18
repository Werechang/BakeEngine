
#include "Renderbuffer.h"

Renderbuffer::Renderbuffer(int width, int height, int samples, int type, int attachmentType) : samples(samples), type(type), attachmentType(attachmentType) {
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    resize(width, height);
}

Renderbuffer::~Renderbuffer() {
    glDeleteRenderbuffers(1, &renderBuffer);
}

void Renderbuffer::attach() const {
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, renderBuffer);
}

void Renderbuffer::resize(int newWidth, int newHeight) const {
    if (samples > 1) {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, type, newWidth, newHeight);
    } else {
        glRenderbufferStorage(GL_RENDERBUFFER, type, newWidth, newHeight);
    }
}
