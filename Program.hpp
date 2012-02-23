#ifndef __Program_H__
#define __Program_H__

#include "Config.hpp"
#include <cstring>

class Program {
public:
    enum Type {
        MASKING_STANDARD,
        RENDER_STANDARD,
        MAX_TYPE
    };

private:
    GLuint m_glId;
    static Program* s_pCurrentProgram;
    
    const char *loadFromFile(const std::string& fn) const;    
    
public:
    Program() {};
    Program(const std::string& vertexPath, const std::string& fragmentPath);
    ~Program();
    
    void active();
    static void deactive();
    void setTexture(const std::string& name, const size_t& value) const;
    void setFloat(const std::string& name, const float& value) const;
    static const Program* getCurrent();
};

#endif // __Program_H__