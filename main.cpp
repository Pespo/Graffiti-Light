#include "glew/glew.h"
#include "headers.hpp"
#include <cstdlib>
#include <cstdio>

#define WINDOW_WIDTH 800;
#define WINDOW_HEIGHT 600;

void initSDLOpenGL();
void initFBOs();
void resize(GLuint w, GLuint h);
void handleUserEvent(const SDL_Event& event);
void handleKeyEvent(const SDL_keysym& keysym, bool down);
void animate();
void handleEvent(const SDL_Event& event);
void renderFrame();
void printGlErrors();
void loop();
void initTimers();
static inline Uint32 genericTimer(Uint32 interval, void* param);

size_t fullScreenWidth;
size_t fullScreenHeight;
size_t width;
size_t height;
size_t windowedWidth;
size_t windowedHeight;
static int timerId = 1;

GLuint frameCount;
uint64_t m_LastStartTime;
uint64_t m_FrameDuration;
SDL_TimerID animateTimer;

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
    loop();
    return 0;
}

void initTimers() {
    animateTimer = SDL_AddTimer(20, genericTimer, &timerId);
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

void resize(GLuint w, GLuint h) {
    
    width = w;
    height = h;
    
    pDrawContext = SDL_SetVideoMode(width, height, 0, videoModeFlags);

    glViewport(0, 0, width, height);
}

void handleUserEvent(const SDL_Event& event) {  
    if (event.user.code == timerId) {
        animate();
        renderFrame();
        
        printGlErrors();
        
        ++frameCount;
        if (frameCount % 20 == 0) {
            //uint64_t time = getTime();
            //m_FrameDuration = (time - m_LastStartTime) / 20LL;
            //m_LastStartTime = time;
        }
    }
}

void handleKeyEvent(const SDL_keysym& keysym, bool down) {
    if (down) {
        switch (keysym.sym) {
            case SDLK_ESCAPE:
                bRunning = false;
                break;
            default:
                break;
        }
    }
}

void animate() {
}

void handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_USEREVENT:
            handleUserEvent(event);
            break;
        case SDL_KEYDOWN:
            handleKeyEvent(event.key.keysym, true);
            break;
        case SDL_KEYUP:
            handleKeyEvent(event.key.keysym, false);
            break;
        case SDL_QUIT:
            bRunning = false;
            break;
        default:
            break;
    }
}

void renderFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    //m_Scene.drawObjectsOfScene();
    
    SDL_GL_SwapBuffers();
}

static inline Uint32 genericTimer(Uint32 interval, void* param) {
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = *static_cast<int*>(param);
    event.user.data1 = 0;
    event.user.data2 = 0;
    SDL_PushEvent(&event);
    return interval;
}

void printGlErrors() {
    GLenum error = glGetError();
    
    while (error != GL_NO_ERROR) {
        cout << "!! GL Error : ";
        if (error == GL_INVALID_ENUM)
            cout << "GL_INVALID_ENUM" << endl;
        if (error == GL_INVALID_VALUE)
            cout << "GL_INVALID_VALUE" << endl;
        if (error == GL_INVALID_OPERATION)
            cout << "GL_INVALID_OPERATION" << endl;
        if (error == GL_OUT_OF_MEMORY)
            cout << "GL_OUT_OF_MEMORY" << endl;
        
        error = glGetError();
        
        cout << endl;
    }
}

void loop() {
    initTimers();
    //m_LastStartTime = getTime();
    SDL_Event event;
    
    while (bRunning) {
        while (SDL_PollEvent(&event))
            handleEvent(event); // Checks each event for types and loach corresponding actions
        printGlErrors();
    }
}

void initFBOs() {
    
}