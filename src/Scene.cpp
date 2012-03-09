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
        
    m_pDrawBuffer = new VBO(drawCoord, 4, 4);
}


Scene::Scene(SDL_Surface* gl) :
    m_pContext(gl),
    m_width(gl->w),
    m_height(gl->h) {
    float drawCoord[4 * 4] = {-1.,  1., 0., 0.,
                              -1., -1., 0., 1.,
                               1., -1., 1., 1.,
                               1.,  1., 1., 0.};
    
    m_pDrawBuffer = new VBO(drawCoord, 4, 4);
}

Scene::~Scene() {
    //delete m_pContext;
}

void Scene::render() const {
    if (LM_DEBUG) cout << "Scene : render" << endl;
    m_pDrawBuffer->bind();
    
    m_pDrawBuffer->attribPointer("vertexPos", 2, 0);
    m_pDrawBuffer->attribPointer("texturePos", 2, 2);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    m_pDrawBuffer->unbind();
}


const size_t& Scene::width() const {
    return m_width;
}

const size_t& Scene::height() const {
    return m_height;
}

VBO* Scene::getVBO() const {
    return m_pDrawBuffer;
}

void Scene::resize(const size_t& w, const size_t& h) {
    m_width = w;
    m_height = h;
    m_pContext = SDL_SetVideoMode(m_width, m_height, 0, SDL_OPENGL | SDL_RESIZABLE);
    glViewport(0, 0, m_width, m_height);
}