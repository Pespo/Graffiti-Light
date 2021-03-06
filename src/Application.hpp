#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Config.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "Program.hpp"
#include "FBO.hpp"
#include "Mask.hpp"
#include "OpenGL.hpp"
#include "Mask.hpp"
#include "Timer.hpp"
#include <map>

class Application {
    
    /*const*/ Scene m_scene;
    Camera m_camera;
    FBO* m_fbo;
    PingPong m_masks;
    float m_threshold;
	float m_time;
	float m_timeSave;
	unsigned int m_resetCheck;
    bool m_bRunning;
    std::map<Program::Type, Program*> m_pPrograms;

    
    Program m_programsMasking;
    Program m_programsRender;
    
    void handleKeyEvent(const SDL_keysym& keysym, bool down);
    void handleEvent(const SDL_Event& event);
    void handleUserEvent(const SDL_Event& event);
    void render();
    void printTransferRate(); 
public:
    Application(SDL_Surface* GLContext);
    ~Application();
    void init();
    void run();
    void renderOffScreen();
    void renderOnScreen();
};

#endif // __APPLICATION_H__