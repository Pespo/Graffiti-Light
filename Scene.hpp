#ifndef __SCENE_H__
#define __SCENE_H__

#include "Config.hpp"
#include "VBO.hpp"

class Scene {
    SDL_Surface* m_pContext;
    size_t m_width;
    size_t m_height;
    VBO m_drawSurface;
    
public:
    Scene(SDL_Surface* GLContext);
    Scene();
    ~Scene();
    void render() const;
    const size_t& width() const;
    const size_t& height() const;
};

#endif // __SCENE_H__