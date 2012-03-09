#include "OpenGL.hpp"
#include <iostream>

using namespace std;

void OpenGL::printErrors() {
    GLenum error = glGetError();

    while (error != GL_NO_ERROR) {
        cout << "!! GL_ERROR : " ;
        if (error == GL_INVALID_ENUM)
            cout << "GL_INVALID_ENUM" << endl;
        if (error == GL_INVALID_VALUE)
            cout << "GL_INVALID_VALUE" << endl;
        if (error == GL_INVALID_OPERATION)
            cout << "GL_INVALID_OPERATION" << endl;
        if (error == GL_OUT_OF_MEMORY)
            cout << "GL_OUT_OF_MEMORY" << endl;
        if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
            cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl;
        
        error = glGetError();
    }
}

void OpenGL::clearColor() {
    glClear(GL_COLOR_BUFFER_BIT);
}
