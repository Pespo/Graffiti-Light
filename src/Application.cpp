#include "Application.hpp"
#include <iostream>

using namespace std;


Application::Application(SDL_Surface* GLContext) :
    m_scene(GLContext),
    m_camera(CV_CAP_ANY),
//    m_camera("/Users/Tom/Desktop/1.mp4"),
    m_masks(),
    m_threshold(1),
    m_bRunning(true) {
    cout << "Application : new" << endl;
    m_scene.resize(m_camera.get(Camera::WIDTH), m_camera.get(Camera::HEIGHT));
    cout << "Scene : width = " << m_scene.width() <<  endl;
    cout << "Scene : height = " << m_scene.height() << endl;
    
}

Application::~Application() {

}

void Application::init() {
    cout << "Application : init" << endl;
    // =====================================================
    // =     Init fbo with two targets (color & timer)     =
    // =====================================================
    const GLenum fboTargets[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    m_fbo = new FBO(2, fboTargets);
    
    // =======================
    // =     Init mask     =
    // =======================
    // Color masks
    unsigned char* void4b = new unsigned char[4 * m_scene.width() * m_scene.height()];
    memset(void4b, 0, 4 * m_scene.width() * m_scene.height() * sizeof(unsigned char));
    
    m_masks.pIn->pColor->bind();
    m_masks.pIn->pColor->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pColor->unbind();
    
    m_masks.pOut->pColor->bind();
    m_masks.pOut->pColor->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pColor->unbind();
    
    delete [] void4b;
    
    // Timer masks
    unsigned short* void1s = new unsigned short[m_scene.width() * m_scene.height()];
    memset(void1s, 0, m_scene.width() * m_scene.height() * sizeof(unsigned short));

    m_masks.pIn->pTimer->bind();
    m_masks.pIn->pTimer->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pTimer->unbind();

    m_masks.pOut->pTimer->bind();
    m_masks.pOut->pTimer->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pTimer->unbind();

    delete [] void1s;

    // =======================
    // =     Init camera     =
    // =======================
    m_camera.capture();

    // ========================
    // =     Init shaders     =
    // ========================

	#ifdef LM_WINDOWS
		m_pPrograms[Program::MASKING_STANDARD] = new Program("shaders/maskDarwin.vert", "shaders/maskDarwin.frag");
		m_pPrograms[Program::RENDER_STANDARD] = new Program("shaders/compoDarwin.vert", "shaders/compoDarwin.frag");
	#endif
    
	#ifdef LM_APPLE
        m_pPrograms[Program::MASKING_STANDARD] = new Program("shaders/maskDarwin.vert", "shaders/maskDarwin.frag");
        m_pPrograms[Program::RENDER_STANDARD] = new Program("shaders/compoDarwin.vert", "shaders/compoDarwin.frag");
    #endif
}

void Application::run() {
    if (LM_DEBUG) cout << "Application : run" << endl;
    SDL_Event event;
    while (m_bRunning) {
        render();
        while (SDL_PollEvent(&event))
            handleEvent(event);
        OpenGL::printErrors();
    }
}

void Application::render() {
    if (LM_DEBUG) cout <<"Application : render" << endl;
    
    m_camera.capture();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderOffScreen();
    
    renderOnScreen();
        
    m_masks.swap();
    
    SDL_GL_SwapBuffers();
}

void Application::renderOnScreen() {
    FBO::unbind();
    
    m_pPrograms[Program::RENDER_STANDARD]->active();
    
    m_masks.pOut->pColor->bindOn(GL_TEXTURE3);
    
    Program::getCurrent()->setTexture("textureCam", 0);
    Program::getCurrent()->setTexture("textureMask", 3);
    
    m_scene.render();
}

void Application::renderOffScreen() {
    m_camera.getTexture().bindOn(GL_TEXTURE0);
    
    m_pPrograms[Program::MASKING_STANDARD]->active();
    
    m_fbo->bind();
    
    m_masks.pIn->pColor->bindOn(GL_TEXTURE1);
    m_masks.pIn->pTimer->bindOn(GL_TEXTURE2);
    
    m_fbo->attachTexture(m_masks.pOut->pColor, GL_COLOR_ATTACHMENT0);
    m_fbo->attachTexture(m_masks.pOut->pTimer, GL_COLOR_ATTACHMENT1);
    
    Program::getCurrent()->setTexture("textureCam", 0);
    Program::getCurrent()->setTexture("textureMask", 1);
    Program::getCurrent()->setTexture("timingMask", 2);
    Program::getCurrent()->setFloat("threshold", m_threshold);
    
    m_scene.render();
}

void Application::handleKeyEvent(const SDL_keysym& keysym, bool down) {
    if (down) {
        switch (keysym.sym) {
            case SDLK_ESCAPE :
                m_bRunning = false;
                break;
            case SDLK_r: {
                unsigned char* void4b = new unsigned char[4 * m_scene.width() * m_scene.height()];
                memset(void4b, 1, 4 * m_scene.width() * m_scene.height() * sizeof(unsigned char));
                
                m_masks.pIn->pColor->bind();
                m_masks.pIn->pColor->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
                m_masks.pIn->pColor->unbind();
                
                m_masks.pOut->pColor->bind();
                m_masks.pOut->pColor->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
                m_masks.pOut->pColor->unbind();
                
                delete [] void4b;
                cout << "Application : reset mask" << endl;
                } break;
            /* Increase threshold */
            case SDLK_p:
                cout << "Application : increase threshold";
                m_threshold += 0.05;
                if (m_threshold > 1)
                    m_threshold = 1;
                cout << " to " << m_threshold << endl;
                break;
            /* Decrease threshold */
            case SDLK_m:
                cout << "Application : decrease threshold";
                m_threshold -= 0.05;
                if (m_threshold < 0)
                    m_threshold = 0;
                cout << " to " << m_threshold << endl;
                break;
            /* Active record */
            case SDLK_s:
                
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
        case SDL_VIDEORESIZE: {
            m_scene.resize(event.resize.w, event.resize.h);
            
            // Color masks
            unsigned char* void4b = new unsigned char[4 * m_scene.width() * m_scene.height()];
            memset(void4b, 1, 4 * m_scene.width() * m_scene.height() * sizeof(unsigned char));
            
            m_masks.pIn->pColor->bind();
            m_masks.pIn->pColor->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
            m_masks.pIn->pColor->unbind();
            
            m_masks.pOut->pColor->bind();
            m_masks.pOut->pColor->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
            m_masks.pOut->pColor->unbind();
            
            delete [] void4b;
            
            // Timer masks
            unsigned short* void1s = new unsigned short[m_scene.width() * m_scene.height()];
            memset(void1s, 1, m_scene.width() * m_scene.height() * sizeof(unsigned short));
            
            m_masks.pIn->pTimer->bind();
            m_masks.pIn->pTimer->attachData(void1s, m_scene.width(), m_scene.height(), GL_RED, GL_UNSIGNED_SHORT, GL_RED);
            m_masks.pIn->pTimer->unbind();
            
            m_masks.pOut->pTimer->bind();
            m_masks.pOut->pTimer->attachData(void1s, m_scene.width(), m_scene.height(), GL_RED, GL_UNSIGNED_SHORT, GL_RED);
            m_masks.pOut->pTimer->unbind();
            
            delete [] void1s;
            } break;
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
