#include "Scene.hpp"
#include "Program.hpp"

Scene::Scene() :
    m_width(),
    m_height() {
    float drawCoord[4 * 4] = {-1.,  1., 0., 0.,
                              -1., -1., 0., 1.,
                               1., -1., 1., 1.,
                               1.,  1., 1., 0.};
        
    m_drawSurface = VBO(drawCoord, 4, 4);
}


Scene::Scene(SDL_Surface* gl) :
    m_pContext(gl),
    m_width(gl->w),
    m_height(gl->h) {
    float drawCoord[4 * 4] = {-1.,  1., 0., 0.,
                              -1., -1., 0., 1.,
                               1., -1., 1., 1.,
                               1.,  1., 1., 0.};
    
    m_drawSurface = VBO(drawCoord, 4, 4);
}

Scene::~Scene() {
    //delete m_pContext;
}

void Scene::render() const {
    m_drawSurface.bind();
    m_drawSurface.attribPointer("vertPosition", 2, 0);
    m_drawSurface.attribPointer("textPosition", 2, 2);
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    m_drawSurface.unbind();
}


const size_t& Scene::width() const {
    return m_width;
}

const size_t& Scene::height() const {
    return m_height;
}