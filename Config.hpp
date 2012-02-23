#ifndef __CONFIG_H__
#define __CONFIG_H__



// ==================================
// =     Define platform macros     =
// ==================================
#if defined _WIN64 || defined _WIN32
    #define MUXOGL_WINDOWS
#elif __APPLE__
    #define MUXOGL_APPLE
#elif __linux
    #define MUXOGL_LINUX
#endif

// ============================
// =     library includes     =
// ============================
#include <opencv2/opencv.hpp>
    
#ifdef MUXOGL_WINDOWS
    #include <GL/glew.h>
    #include <GL/GL.h>
    #include <GL/GLU.h>
    #include <sdl-1.3/SDL.h>
#endif
    
#ifdef MUXOGL_APPLE
    #include <SDL/SDL.h>
    #include <SDL/SDL_main.h>
    #include "glew/glew.h"
    #include <OpenGL/gl.h>
    #include <OpenGL/OpenGL.h>
#endif

#endif // __CONFIG_H__