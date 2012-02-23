#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Config.hpp"

class Texture {
    GLuint m_glId;
    
public:
    Texture();
    Texture(const GLenum minFilter, const GLenum maxFilter);
    Texture(Texture& copy);
    ~Texture();
    
    void bind() const;
    void bindOn(const GLenum& active = GL_TEXTURE0) const;
    static void unbind();
    static void attachData(const GLvoid* data, const GLsizei& w, const GLsizei& h, const GLenum& dataColorFormat, const GLenum& dataType, const GLint& internColorFormat = GL_RGB);
    const GLuint& getId() const;
};

#endif // __TEXTURE_H__