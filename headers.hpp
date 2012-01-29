#ifndef __HEADERS_HPP__
#define __HEADERS_HPP__

#if defined _WIN64 || defined _WIN32

	#include <opencv2/opencv.hpp>
	#include <GL/glew.h>
	#include <GL/GL.h>
	#include <GL/GLU.h>
	#include <sdl-1.3/SDL.h>
	

#elif __APPLE__
#include <opencv2/opencv.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_main.h>
#include "glew/glew.h"
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