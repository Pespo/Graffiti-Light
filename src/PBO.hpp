#ifndef __PBO_H__
#define __PBO_H__

#include "Config.hpp"
#include "Texture.hpp"

class PBO {
    GLuint m_glId;
    
public:
    PBO();
    ~PBO();
    
    void bind() const;
    static void unbind();
    const GLuint& getId() const;
};

#endif // __PBO_H__