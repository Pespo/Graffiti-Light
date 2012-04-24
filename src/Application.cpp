#include "Application.hpp"
#include <iostream>

using namespace std;


Application::Application(SDL_Surface* GLContext) :
    m_scene(GLContext),
    m_camera(1),
//    m_camera("/Users/Tom/Desktop/1.mp4"),
    m_masks(),
    m_threshold(1),
	m_time(0.09),
	m_timeSave(0),
	m_resetCheck(0),
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
    const GLenum fboTargets[8] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7/*, GL_COLOR_ATTACHMENT8/*, GL_COLOR_ATTACHMENT9*/};
    m_fbo = new FBO(8, fboTargets);
    
    // =======================
    // =     Init mask     =
    // =======================
    // Color masks
    unsigned char* void4b = new unsigned char[4 * m_scene.width() * m_scene.height()];
    memset(void4b, 0, 4 * m_scene.width() * m_scene.height() * sizeof(unsigned char));
    
	m_masks.pIn->pColor0->bind();
    m_masks.pIn->pColor0->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pColor0->unbind();
    
    m_masks.pOut->pColor0->bind();
    m_masks.pOut->pColor0->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pColor0->unbind();

    m_masks.pIn->pColor1->bind();
    m_masks.pIn->pColor1->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pColor1->unbind();
    
    m_masks.pOut->pColor1->bind();
    m_masks.pOut->pColor1->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pColor1->unbind();

	m_masks.pIn->pColor2->bind();
    m_masks.pIn->pColor2->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pColor2->unbind();
    
    m_masks.pOut->pColor2->bind();
    m_masks.pOut->pColor2->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pColor2->unbind();
	
	m_masks.pIn->pColor3->bind();
    m_masks.pIn->pColor3->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pColor3->unbind();
    
    m_masks.pOut->pColor3->bind();
    m_masks.pOut->pColor3->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pColor3->unbind();

	m_masks.pIn->pColor4->bind();
    m_masks.pIn->pColor4->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pColor4->unbind();
    
    m_masks.pOut->pColor4->bind();
    m_masks.pOut->pColor4->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pColor4->unbind();

	m_masks.pIn->pColor5->bind();
    m_masks.pIn->pColor5->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pColor5->unbind();
    
    m_masks.pOut->pColor5->bind();
    m_masks.pOut->pColor5->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pColor5->unbind();

	/*m_masks.pIn->pColor6->bind();
    m_masks.pIn->pColor6->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pColor6->unbind();
    
    m_masks.pOut->pColor6->bind();
    m_masks.pOut->pColor6->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pColor6->unbind();
	
	/*m_masks.pIn->pColor7->bind();
    m_masks.pIn->pColor7->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pColor7->unbind();
    
    m_masks.pOut->pColor7->bind();
    m_masks.pOut->pColor7->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pColor7->unbind();*/
    
  //  delete [] void4b;
    
    // Timer masks
	//unsigned short* void1s = new unsigned short[m_scene.width() * m_scene.height()];
   // memset(void1s, 0, m_scene.width() * m_scene.height() * sizeof(unsigned short));

    m_masks.pIn->pTimer0->bind();
    m_masks.pIn->pTimer0->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pTimer0->unbind();

    m_masks.pOut->pTimer0->bind();
    m_masks.pOut->pTimer0->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pTimer0->unbind();

	m_masks.pIn->pTimer1->bind();
    m_masks.pIn->pTimer1->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pIn->pTimer1->unbind();

    m_masks.pOut->pTimer1->bind();
    m_masks.pOut->pTimer1->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
    m_masks.pOut->pTimer1->unbind();

 //   delete [] void1s;
	delete [] void4b;

    // =======================
    // =     Init camera     =
    // =======================
    m_camera.capture();

    // ========================
    // =     Init shaders     =
    // ========================

    char cCurrentPath[FILENAME_MAX];
    char vertpath[FILENAME_MAX];
	char fragpath[FILENAME_MAX];
    
    GetCurrentDir(cCurrentPath, sizeof(cCurrentPath) / sizeof(char));
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';

    sprintf(vertpath, "%s/%s", cCurrentPath, "src/shaders/maskDarwin.vert");
    sprintf(fragpath, "%s/%s", cCurrentPath, "src/shaders/maskDarwin.frag");
    m_pPrograms[Program::MASKING_STANDARD] = new Program(vertpath, fragpath);

    sprintf(vertpath, "%s/%s", cCurrentPath, "src/shaders/compoDarwin.vert");
    sprintf(fragpath, "%s/%s", cCurrentPath, "src/shaders/compoDarwin.frag");
    m_pPrograms[Program::RENDER_STANDARD] = new Program(vertpath, fragpath);
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
    
    m_masks.pOut->pColor0->bindOn(GL_TEXTURE11);
	m_masks.pOut->pColor1->bindOn(GL_TEXTURE12);
	m_masks.pOut->pColor2->bindOn(GL_TEXTURE13);
	m_masks.pOut->pColor3->bindOn(GL_TEXTURE14);
	m_masks.pOut->pColor4->bindOn(GL_TEXTURE15);
	m_masks.pOut->pColor5->bindOn(GL_TEXTURE16);
	//m_masks.pOut->pColor6->bindOn(GL_TEXTURE17);
	//m_masks.pOut->pColor7->bindOn(GL_TEXTURE18);
    
    Program::getCurrent()->setTexture("textureCam", 0);
    Program::getCurrent()->setTexture("textureMask0", 11);
	Program::getCurrent()->setTexture("textureMask1", 12);
    Program::getCurrent()->setTexture("textureMask2", 13);
	Program::getCurrent()->setTexture("textureMask3", 14);
	Program::getCurrent()->setTexture("textureMask4", 15);
	Program::getCurrent()->setTexture("textureMask5", 16);
	//Program::getCurrent()->setTexture("textureMask6", 17);
	//Program::getCurrent()->setTexture("textureMask7", 18);
    m_scene.render();
}

void Application::renderOffScreen() {
    m_camera.getTexture().bindOn(GL_TEXTURE0);
    
    m_pPrograms[Program::MASKING_STANDARD]->active();
    
    m_fbo->bind();
    
    m_masks.pIn->pColor0->bindOn(GL_TEXTURE1);
	m_masks.pIn->pColor1->bindOn(GL_TEXTURE2);
	m_masks.pIn->pColor2->bindOn(GL_TEXTURE3);
	m_masks.pIn->pColor3->bindOn(GL_TEXTURE4);
	m_masks.pIn->pColor4->bindOn(GL_TEXTURE5);
	m_masks.pIn->pColor5->bindOn(GL_TEXTURE6);
	//m_masks.pIn->pColor6->bindOn(GL_TEXTURE7);
	//m_masks.pIn->pColor7->bindOn(GL_TEXTURE8);
    m_masks.pIn->pTimer0->bindOn(GL_TEXTURE7);
	m_masks.pIn->pTimer1->bindOn(GL_TEXTURE8);
    
    m_fbo->attachTexture(m_masks.pOut->pColor0, GL_COLOR_ATTACHMENT0);
	m_fbo->attachTexture(m_masks.pOut->pColor1, GL_COLOR_ATTACHMENT1);
	m_fbo->attachTexture(m_masks.pOut->pColor2, GL_COLOR_ATTACHMENT2);
	m_fbo->attachTexture(m_masks.pOut->pColor3, GL_COLOR_ATTACHMENT3);
	m_fbo->attachTexture(m_masks.pOut->pColor4, GL_COLOR_ATTACHMENT4);
	m_fbo->attachTexture(m_masks.pOut->pColor5, GL_COLOR_ATTACHMENT5);
	//m_fbo->attachTexture(m_masks.pOut->pColor6, GL_COLOR_ATTACHMENT6);
	//m_fbo->attachTexture(m_masks.pOut->pColor7, GL_COLOR_ATTACHMENT7);
    m_fbo->attachTexture(m_masks.pOut->pTimer0, GL_COLOR_ATTACHMENT6);
	m_fbo->attachTexture(m_masks.pOut->pTimer1, GL_COLOR_ATTACHMENT7);
    
    Program::getCurrent()->setTexture("textureCam", 0);
    Program::getCurrent()->setTexture("textureMask0", 1);
	Program::getCurrent()->setTexture("textureMask1", 2);
	Program::getCurrent()->setTexture("textureMask2", 3);
	Program::getCurrent()->setTexture("textureMask3", 4);
	Program::getCurrent()->setTexture("textureMask4", 5);
	Program::getCurrent()->setTexture("textureMask5", 6);
	//Program::getCurrent()->setTexture("textureMask6", 7);
	//Program::getCurrent()->setTexture("textureMask7", 8);
    Program::getCurrent()->setTexture("timingMask0", 7);
	Program::getCurrent()->setTexture("timingMask1", 8);

    Program::getCurrent()->setFloat("threshold", m_threshold);
	Program::getCurrent()->setFloat("time", m_time);
	if (m_resetCheck > 0) {
		m_resetCheck --;
		if(m_resetCheck == 0) {
			m_time = m_timeSave;
		}
	}
    
    m_scene.render();
}

void Application::handleKeyEvent(const SDL_keysym& keysym, bool down) {
    if (down) {
        switch (keysym.sym) {
            case SDLK_ESCAPE :
                m_bRunning = false;
                break;
            case SDLK_r: {
				m_timeSave = m_time;
                m_time = 0;
				m_resetCheck = 7;
                cout << "Application : reset mask" << endl;
                } break;
            /* Increase threshold */
            case SDLK_p:
            case SDLK_z:
                cout << "Application : increase threshold";
                m_threshold += 0.05;
                if (m_threshold > 1)
                    m_threshold = 1;
                cout << " to " << m_threshold << endl;
                break;
            /* Decrease threshold */
            case SDLK_m:
            case SDLK_s:
                cout << "Application : decrease threshold";
                m_threshold -= 0.05;
                if (m_threshold < 0)
                    m_threshold = 0;
                cout << " to " << m_threshold << endl;
                break;
			/* Increase time */
			case SDLK_e:
				cout << "Application : increase time";
				m_time += 0.005;
				if (m_time > 1)
					m_time = 1;
				cout << " to " << m_time << endl;
				break;
			case SDLK_d:
				cout << "Application : decrease time";
				m_time -= 0.005;
				if (m_time < 0)
					m_time = 0;
				cout << " to " << m_time << endl;
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
            
            m_masks.pIn->pColor1->bind();
            m_masks.pIn->pColor1->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
            m_masks.pIn->pColor1->unbind();
            
            m_masks.pOut->pColor1->bind();
            m_masks.pOut->pColor1->attachData(void4b, m_scene.width(), m_scene.height(), GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA);
            m_masks.pOut->pColor1->unbind();
            
            delete [] void4b;
            
            // Timer masks
            unsigned short* void1s = new unsigned short[m_scene.width() * m_scene.height()];
            memset(void1s, 1, m_scene.width() * m_scene.height() * sizeof(unsigned short));
            
            m_masks.pIn->pTimer1->bind();
            m_masks.pIn->pTimer1->attachData(void1s, m_scene.width(), m_scene.height(), GL_RED, GL_UNSIGNED_SHORT, GL_RED);
            m_masks.pIn->pTimer1->unbind();
            
            m_masks.pOut->pTimer1->bind();
            m_masks.pOut->pTimer1->attachData(void1s, m_scene.width(), m_scene.height(), GL_RED, GL_UNSIGNED_SHORT, GL_RED);
            m_masks.pOut->pTimer1->unbind();
            
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
