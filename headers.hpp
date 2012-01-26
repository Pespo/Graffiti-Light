#ifndef __HEADERS_HPP__
#define __HEADERS_HPP__
	
	#include "functions.hpp"

	#if defined _WIN64 || defined _WIN32
		#include "targetver.h"
		#include <windows.h> 
		#include <opencv2/opencv.hpp>
		#include <tchar.h>
		#include <cstdlib>
		#include <glew.h>			// OpenGL Extension "autoloader"
		//#include <glut.h>
		#include <GL.h>
		#include <GLU.h>
		#include <SDL/SDL.h>
		#include <SDL/SDL_main.h>
		
		//#include <gl.h>	
		#undef main
	#elif __APPLE__
		#include <opencv2/opencv.hpp>
	#elif __linux
		// linux
	#elif __unix // all unices not caught above
		// Unix
	#elif __posix
		// POSIX
	#endif
	
	#include <cassert>
	#include <iostream>
	#include <cstdio>
	#include <ctime>
	#include <cstdio>

#endif





// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme
