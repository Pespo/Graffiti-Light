#include "Application.hpp"
#include <iostream>

using namespace std;

Application::Application(SDL_Surface* GLContext) :
    m_scene(GLContext),
    m_camera(CV_CAP_ANY),
    m_masks(),
    m_threshold(),
    m_bRunning(false) {
    cout << "new Application" << endl;
}

Application::~Application() {
    
}

void Application::init() {
    cout << "init Application" << endl;
    // =====================================================
    // =     Init fbo with two targets (color & timer)     =
    // =====================================================
    
    const GLenum fboTargets[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    m_fbo = FBO(2, fboTargets);
    
    // =======================
    // =     Init mask     =
    // =======================

    // Timer masks
    
    unsigned char* void4b = new unsigned char[4 * m_scene.width() * m_scene.height()];
    memset(void4b, 1, 4 * m_scene.width() * m_scene.height() * sizeof(unsigned char));
    cout << "OK : memset void4b" << endl;
    
    m_masks.in.color.bind();
    m_masks.in.color.attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.in.color.unbind();
    cout << "OK : init mask in.color" << endl;
    
    m_masks.out.color.bind();
    m_masks.out.color.attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.out.color.unbind();
    cout << "OK : init mask out.color" << endl;
    
    delete [] void4b;
    cout << "OK : delete void4b" << endl;
    
    // Color masks
    unsigned short* void1s = new unsigned short[m_scene.width() * m_scene.height()];
    memset(void1s, 1, m_scene.width() * m_scene.height() * sizeof(unsigned short));
    cout << "OK : memset void1s" << endl;

    m_masks.in.timer.bind();
    m_masks.in.timer.attachData(void1s, m_scene.width(), m_scene.height(), GL_RED, GL_UNSIGNED_SHORT, GL_RED);
    m_masks.in.timer.unbind();
    cout << "OK : init mask int.timer" << endl;

    m_masks.out.timer.bind();
    m_masks.out.timer.attachData(void1s, m_scene.width(), m_scene.height(), GL_RED, GL_UNSIGNED_SHORT, GL_RED);
    m_masks.out.timer.unbind();
    cout << "OK : init mask out.timer" << endl;

    delete [] void1s;
    cout << "OK : delete void1s" << endl;

    // =======================
    // =     Init camera     =
    // =======================

    m_camera.capture();
    cout << "OK : camera capture" << endl;

    // ========================
    // =     Init shaders     =
    // ========================

    m_programs[Program::MASKING_STANDARD] = Program("shaders/maskDarwin.vert", "shaders/maskDarwin.frag");
    cout << "OK : init masking program" << endl;

    m_programs[Program::RENDER_STANDARD] = Program("shaders/compoDarwin.vert", "shaders/compoDarwin.frag");
    cout << "OK : init render program" << endl;
}

void Application::run() {
    SDL_Event event;
    while (m_bRunning) {
        render();
        while (SDL_PollEvent(&event))
            handleEvent(event);
        OpenGL::printErrors();
    }
}

void Application::render() {
    m_camera.capture();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    // =============================
    // =     Render off screen     =
    // =============================
    m_programs[Program::MASKING_STANDARD].active();
    m_fbo.bind();

        m_camera.getTexture().bindOn(0);
        m_masks.in.color.bindOn(1);
        m_masks.in.timer.bindOn(2);
        m_masks.out.color.bindOn(3);
        m_masks.out.timer.bindOn(4);
            
        m_fbo.attachTexture(m_masks.in.color, GL_COLOR_ATTACHMENT0);
        m_fbo.attachTexture(m_masks.in.timer, GL_COLOR_ATTACHMENT1);
        
        m_programs[Program::MASKING_STANDARD].getCurrent()->setTexture("camTexture", 0);
        m_programs[Program::MASKING_STANDARD].getCurrent()->setTexture("pingColorTexture", 1);
        m_programs[Program::MASKING_STANDARD].getCurrent()->setTexture("pingTimeTexture", 2);
        
        m_programs[Program::MASKING_STANDARD].getCurrent()->setFloat("seuilIn", m_threshold);

        m_scene.render();
    
    m_fbo.unbind();
    
    // ============================
    // =     Render on screen     =
    // ============================
    m_programs[Program::RENDER_STANDARD].active();

    m_camera.getTexture().bindOn(0);
    m_masks.out.color.bindOn(1);
    //m_masks.out.timer.bindOn(2);
    
    m_programs[Program::MASKING_STANDARD].getCurrent()->setTexture("camTexture", 0);
    m_programs[Program::MASKING_STANDARD].getCurrent()->setTexture("maskTesture", 1);

    m_scene.render();
    
    m_masks.swap();
    
    SDL_GL_SwapBuffers();
}

void Application::handleKeyEvent(const SDL_keysym& keysym, bool down) {
    if (down) {
        switch (keysym.sym) {
            case SDLK_ESCAPE :
                m_bRunning = false;
                break;
            case SDLK_r :
                // Reset mask
                break;
            case SDLK_p :
                // Increase threshold
                break;
            case SDLK_m :
                // Decrease threshold
                break;
            default :
                break;
        }
    }
}

void Application::handleEvent(const SDL_Event& event) {
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
            m_bRunning = false;
            break;
        default:
            break;
    }
}

void Application::handleUserEvent(const SDL_Event& event) {
    if (event.user.code == 1/*m_loopTimer*/) {
        render();
        OpenGL::printErrors();
    }
}
