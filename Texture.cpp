#include "Texture.hpp"
#include "OpenGL.hpp"
#include <iostream>

using namespace std;

Texture::Texture() : m_glId(0) {

}

Texture::Texture(Texture& copy) : m_glId(copy.m_glId) {
    
}

Texture::Texture(const GLenum minFilter, const GLenum maxFilter) : m_glId(0) {
    cout << "new Texture" << endl;
    glGenTextures(1, &m_glId);
    cout << "Texture generated" << endl;
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    unbind();
}

void Texture::bind() const {
    cout << "Bind Texture : " << m_glId << endl;
    OpenGL::printErrors();
    glBindTexture(GL_TEXTURE_2D, m_glId);
    OpenGL::printErrors();
    cout << "Binded" << endl;
}

void Texture::bindOn(const GLenum& active) const {
    glActiveTexture(active);
    bind();
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::attachData(const GLvoid* data, const GLsizei& w, const GLsizei& h, const GLenum& dataFormat, const GLenum& dataType, const GLint& internFormat) {
    cout << "Width : " << w << endl;
    cout << "Height : " << h << endl;
    glTexImage2D(GL_TEXTURE_2D, 0, internFormat, w, h, 0, dataFormat, dataType, data);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_glId);
}

const GLuint& Texture::getId() const {
    return m_glId;
}