#include "Scene.hpp"
#include "Program.hpp"
#include "OpenGL.hpp"
#include <iostream>

using namespace std;

Scene::Scene() :
    m_width(),
    m_height() {
    float drawCoord[4 * 4] = {-1.,  1., 0., 0.,
                              -1., -1., 0., 1.,
                               1., -1., 1., 1.,
                               1.,  1., 1., 0.};
        
    m_drawBuffer = new VBO(drawCoord, 4, 4);
}


Scene::Scene(SDL_Surface* gl) :
    m_pContext(gl),
    m_width(gl->w),
    m_height(gl->h) {
    float drawCoord[4 * 4] = {-1.,  1., 0., 0.,
                              -1., -1., 0., 1.,
                               1., -1., 1., 1.,
                               1.,  1., 1., 0.};
    
    m_drawBuffer = new VBO(drawCoord, 4, 4);
}

Scene::~Scene() {
    //delete m_pContext;
}

void Scene::render() const {
    cout << "Scene : render" << endl;
    m_drawBuffer->bind();
    
    m_drawBuffer->attribPointer("vertPosition", 2, 0);
    m_drawBuffer->attribPointer("textPosition", 2, 2);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    m_drawBuffer->unbind();
}


const size_t& Scene::width() const {
    return m_width;
}

const size_t& Scene::height() const {
    return m_height;
}

VBO* Scene::getVBO() const {
    return m_drawBuffer;
}