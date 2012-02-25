#include "VBO.hpp"
#include <iostream>

using namespace std;

VBO::VBO() : m_widthData(0), m_HeightData(0) {
    glGenBuffers(1, &m_glId);
}

VBO::VBO(const float* data, const size_t& w, const size_t& h, const GLenum& mode) :
    m_widthData(w), m_HeightData(h) { 
    cout << "VBO : new" << endl;
    glGenBuffers(1, &m_glId);
    bind();
    glBufferData(GL_ARRAY_BUFFER, w * h * sizeof(float), data, mode);
    unbind();
}

VBO::~VBO() {
    glDeleteBuffers(1, &m_glId);
}

void VBO::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_glId);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::attribPointer(const char* varInShad, const size_t& nValues, const size_t& startValues) const {
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);

    GLuint position = glGetAttribLocation(program, varInShad);
    glEnableVertexAttribArray(position);

    glVertexAttribPointer(position, nValues, GL_FLOAT, GL_FALSE, m_widthData * sizeof(float), (GLvoid*)(startValues * sizeof(float)));
}

const GLuint& VBO::getId() const {
    return m_glId;
}