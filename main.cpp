#include "glew/glew.h"
#include "headers.hpp"
#include <cstdlib>
#include <cstdio>

#define WINDOW_WIDTH 800;
#define WINDOW_HEIGHT 600;

void initSDLOpenGL();
void initFBOs();

size_t fullScreenWidth;
size_t fullScreenHeight;
size_t width;
size_t height;
size_t windowedWidth;
size_t windowedHeight;

bool bRunning;

uint32_t videoModeFlags;

SDL_Surface *pDrawContext;

using namespace std;

int main (int argc, char** argv) {
    pDrawContext = NULL;
    windowedWidth = WINDOW_WIDTH;
    windowedHeight = WINDOW_HEIGHT;
    videoModeFlags = SDL_OPENGL | SDL_RESIZABLE;
    bRunning = true;
        
    // Initialisation of SDL and creation of OpenGL context
    initSDLOpenGL();
    
    return 0;
}

void initSDLOpenGL() {
    
    int sdlError = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdlError < 0)
        cout << "Unable to init SDL : " << SDL_GetError() << endl;
    
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Double buffering
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Depth buffer size of 16-bit
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // Color components size of 8-bit each
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    
    SDL_EnableUNICODE(1);
        
    const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
    fullScreenWidth = videoInfo->current_w;
    fullScreenHeight = videoInfo->current_h;
    
    width = windowedWidth;
    height = windowedHeight;

    pDrawContext = SDL_SetVideoMode(width, height, 0, videoModeFlags);
    
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
        cout << "GLEW Error : " << glewGetErrorString(glewError) << endl;
    
//    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
//    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
//    SDL_WarpMouse(width / 2, height / 2);
//   SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
    
//    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0., 0., 0., 1.);
    
    glLineWidth(2);
}

void initFBOs() {
    
}