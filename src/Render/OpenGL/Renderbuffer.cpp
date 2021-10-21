
#include "Renderbuffer.h"

Renderbuffer::Renderbuffer(int width, int height, int samples, int type) : samples(samples), type(type) {
    glGenRenderbuffers(1, &renderbuffer);
    bind();
    resize(width, height);
    unbind();
}

Renderbuffer::~Renderbuffer() {
    glDeleteRenderbuffers(1, &renderbuffer);
}

void Renderbuffer::resize(int newWidth, int newHeight) const {
    if (samples > 1) {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, type, newWidth, newHeight);
    } else {
        glRenderbufferStorage(GL_RENDERBUFFER, type, newWidth, newHeight);
    }
}

void Renderbuffer::bind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
}

void Renderbuffer::unbind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

uint32_t Renderbuffer::get() const {
    return renderbuffer;
}
