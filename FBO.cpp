#include "FBO.hpp"

FBO::FBO() {
    glGenFramebuffers(1, &m_glId);
}

FBO::FBO(const size_t& nDrawBufs, const GLenum* drawBufs) {
    glGenFramebuffers(1, &m_glId);
    if (!!drawBufs) {
        bind();
        glDrawBuffers(nDrawBufs, drawBufs);
        unbind();
    }
}

FBO::~FBO() {
    glDeleteFramebuffers(1, &m_glId);
}

void FBO::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_glId);
}

bool isComplete() {
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
        return true;
    }
    return false;
}

void FBO::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::attachTexture(const Texture& tex, const GLenum& color) const {
    glFramebufferTexture2D(GL_FRAMEBUFFER, color, GL_TEXTURE_2D, tex.getId(), 0);
}