#ifndef __FBO_H__
#define __FBO_H__

#include "Config.hpp"
#include "Texture.hpp"

class FBO {
    GLuint m_glId;

public:
    FBO();
    FBO(const size_t& nDrawBufs, const GLenum* drawBufs);
    ~FBO();
    
    void bind() const;
    bool isComplete() const;
    static void unbind();
    void attachTexture(const Texture& tex, const GLenum& color = GL_COLOR_ATTACHMENT0) const;
};

#endif // __FBO_H__