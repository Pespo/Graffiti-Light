#include "Scene.hpp"
#include "Program.hpp"
#include <iostream>

using namespace std;

Scene::Scene() :
    m_width(),
    m_height() {
    float drawCoord[4 * 4] = {-1.,  1., 0., 0.,
                              -1., -1., 0., 1.,
                               1., -1., 1., 1.,
                               1.,  1., 1., 0.};
        
    m_drawBuffer = VBO(drawCoord, 4, 4);
}


Scene::Scene(SDL_Surface* gl) :
    m_pContext(gl),
    m_width(gl->w),
    m_height(gl->h) {
    float drawCoord[4 * 4] = {-1,  1, 0, 0,
                              -1, -1, 0, 1,
                               1, -1, 1, 1,
                               1,  1, 1, 0};
    
    m_drawBuffer = VBO(drawCoord, 4, 4);
}

Scene::~Scene() {
    //delete m_pContext;
}

void Scene::render() const {
    cout << "1" << endl;
    m_drawBuffer.bind();
    
/*    // =================
    // =     hacks     =
    // =================
    
    // Get the index of the attribute vars in shader
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
    
    GLuint posLoc = glGetAttribLocation(program, "vertPosition");
    GLuint texLoc = glGetAttribLocation(program, "textPosition");
    
    cout << "posLoc : " << posLoc << endl;
    cout << "texLoc : " << texLoc << endl;
    
    glEnableVertexAttribArray(posLoc);
    glEnableVertexAttribArray(texLoc);
    
    // Define vertex position : 2 values in a range of 4, starting at index 0
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    // Define texture position : 2 values in a range of 4, starting at index 2
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
    
    
    // =================
    // =     hacks     =
    // =================    */
    
    
    
    m_drawBuffer.attribPointer("vertPosition", 2, 0);
    m_drawBuffer.attribPointer("textPosition", 2, 2);
    cout << "2" << endl;

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    cout << "3" << endl;

    m_drawBuffer.unbind();
    cout << "4" << endl;
}


const size_t& Scene::width() const {
    return m_width;
}

const size_t& Scene::height() const {
    return m_height;
}