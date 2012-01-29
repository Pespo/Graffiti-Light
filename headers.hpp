#ifndef __HEADERS_HPP__
#define __HEADERS_HPP__

#ifdef _WIN64
    //define something for Windows (64-bit)
#elif _WIN32
    //define something for Windows (32-bit)
#elif __APPLE__
#include <opencv2/opencv.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>
#elif __linux
    // linux
#elif __unix // all unices not caught above
    // Unix
#elif __posix
    // POSIX
#endif


#endif