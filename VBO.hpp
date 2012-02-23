#ifndef __VBO_H__
#define __VPO_H__

#include "Config.hpp"

class VBO {
    GLuint m_glId;
    
    size_t m_widthData;
    size_t m_HeightData;
    
public:
    VBO();
    VBO(const float* data, const size_t& w, const size_t& h, const GLenum& mode = GL_STATIC_DRAW);
    ~VBO();
    
    void bind() const;
    static void unbind();
    void attribPointer(const char* varInShad, const size_t& nValues, const size_t& startValues) const;
};

#endif // __VBO_H__