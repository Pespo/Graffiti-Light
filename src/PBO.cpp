#include "PBO.hpp"
#include <iostream>

using namespace std;

PBO::PBO(size_t& size) {
    glGenBuffers(1, &m_glId);
    bind();
    initData(size);
    unbind();
}

PBO::~PBO() {

}

void PBO::copyToTex(Texture& tex, size_t& width, size_t& height, GLenum& format = GL_RGBA) const {
    tex.bind();
    bind()    

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, 0);
}

void PBO::updateData(char* data, size_t& size, size_t& width, size_t& height) const {
    
    bind();
    // map the buffer object into client's memory
    // Note that glMapBufferARB() causes sync issue.
    // If GPU is working with this buffer, glMapBufferARB() will wait(stall)
    // for GPU to finish its job. To avoid waiting (stall), you can call
    // first glBufferDataARB() with NULL pointer before glMapBufferARB().
    // If you do that, the previous data in PBO will be discarded and
    // glMapBufferARB() returns a new allocated pointer immediately
    // even if GPU is still working with the previous data.
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_DRAW);
    GLubyte* dst = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    if (dst) {
        /*
         int* ptr = (int*)dst;
         // copy 4 bytes at once
         for(int i = 0; i < IMAGE_HEIGHT; ++i) {
            for(int j = 0; j < IMAGE_WIDTH; ++j) {
                *ptr = color;
                ++ptr;
            }
         }
         */
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    }
    unbind();
}
    
    
    
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, 0, GL_STREAM_DRAW);
}

void PBO::bind() const {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_glId);
}

void PBO::unbind() {
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

const GLuint& PBO::getId() const {
    return m_glId;
}