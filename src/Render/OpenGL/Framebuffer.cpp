
#include "Framebuffer.h"
#include "../../Util/LogHelperBE.h"

Framebuffer::Framebuffer(int width, int height, int samples) : width(width), height(height), samples(samples) {
    glGenFramebuffers(1, &framebuffer);
    unbind();
}

Framebuffer::~Framebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &framebuffer);
}

void Framebuffer::attachRenderbuffer(int type, int attachmentType) {
    bind();
    renderbufferType = type;
    renderbufferAttachmentType = attachmentType;
    renderbuffer = std::make_unique<Renderbuffer>(width, height, samples, type);
    renderbuffer->bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, renderbuffer->get());
}

void Framebuffer::attachTexture(int type, int attachmentType) {
    bind();
    texType = type;
    texAttachmentType = attachmentType;
    texture = std::make_unique<GLTexture>(width, height, GL_NEAREST, samples, type, 1.0f, false);
    texture->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, texture->get(), 0);
}

void Framebuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Framebuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bindTexture(int slot) const {
    if (texture) {
        texture->activate(slot);
    }
}

void Framebuffer::drawTo(const Framebuffer& fb) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb.framebuffer);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Framebuffer::resize(int nWidth, int nHeight) {
    this->width = nWidth;
    this->height = nHeight;
    bind();
    if (texture) {
    texture->resize(nWidth, nHeight);
    }
    if (renderbuffer) {
        renderbuffer->resize(nWidth, nHeight);
    }
}

void Framebuffer::checkStatus() const {
    bind();
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LogHelperBE::pushName("Framebuffer");
        LogHelperBE::error("FrameBuffer is not complete!");
        LogHelperBE::popName();
    }
}
