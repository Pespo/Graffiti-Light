#include "Config.hpp"
#include "Application.hpp"
#include "Scene.hpp"

using namespace std;

int main (int argc, char* argv[]) {
    
    // =============================
    // =     Init SDL + OpenGL     =
    // =============================
    
    int sdlError = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdlError < 0)
        cout << "Unable to init SDL : " << SDL_GetError() << endl;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
    size_t screenWidth = videoInfo->current_w / 2;
    size_t screenHeight = videoInfo->current_h / 2;

    SDL_Surface* GLContext = SDL_SetVideoMode(screenWidth, screenHeight, 0, SDL_OPENGL | SDL_RESIZABLE);
    glViewport(0, 0, screenWidth, screenHeight);
    
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
        cout << "GLEW Error : " << glewGetErrorString(glewError) << endl;
    
    glClearColor(0., 0., 0., 1.);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    
    // =============================
    // =     Start Application     =
    // =============================
    Application app(GLContext);
    app.init();
    app.run();
    
    //delete GLContext;
    return 0;
}