#ifndef __SCENE_H__
#define __SCENE_H__

#include "Config.hpp"
#include "VBO.hpp"

class Scene {
    SDL_Surface* m_pContext;
    size_t m_width;
    size_t m_height;
    VBO* m_pDrawBuffer;
    
public:
    Scene(SDL_Surface* GLContext);
    Scene();
    ~Scene();
    void render() const;
    const size_t& width() const;
    const size_t& height() const;
    VBO* getVBO() const;
    void resize(const size_t& w, const size_t& h);
};

#endif // __SCENE_H__