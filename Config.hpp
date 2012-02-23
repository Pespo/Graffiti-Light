#ifndef __CONFIG_H__
#define __CONFIG_H__



// ==================================
// =     Define platform macros     =
// ==================================
#if defined _WIN64 || defined _WIN32
    #define LM_WINDOWS
#elif __APPLE__
    #define LM_APPLE
#elif __linux
    #define LM_LINUX
#endif

// ============================
// =     library includes     =
// ============================
#include <opencv2/opencv.hpp>
    
#ifdef LM_WINDOWS
    #include <GL/glew.h>
    #include <GL/GL.h>
    #include <GL/GLU.h>
    #include <sdl-1.3/SDL.h>
#endif
    
#ifdef LM_APPLE
    #include <SDL/SDL.h>
    #include <SDL/SDL_main.h>
    #include "glew/glew.h"
    #include <OpenGL/gl.h>
    #include <OpenGL/OpenGL.h>
#endif

#endif // __CONFIG_H__