#include "FBO.hpp"
#include <iostream>

using namespace std;

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
    cout << "FBO : bind" << endl;
    
    GLenum FBOStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (FBOStatus) {
        case GL_FRAMEBUFFER_COMPLETE:
            cout << "GL_FRAMEBUFFER_COMPLETE" << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << endl;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            cout << "GL_FRAMEBUFFER_UNSUPPORTED" << endl;
            break;
        default:
            cout << "GL_FRAMEBUFFER_UNKOWN_ERROR" << endl;
            break;
    }
    
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