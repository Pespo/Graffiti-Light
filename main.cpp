#include "headers.hpp"
#include "textfile.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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
void initHardware(const int w, const int h);

IplImage * current_frame = NULL;
IplImage * capture_frame = NULL;

size_t fullScreenWidth;
size_t fullScreenHeight;
size_t width;
size_t height;
size_t windowedWidth;
size_t windowedHeight;
static int timerId = 1;
GLuint camTexture;

GLuint shaderCompo;
GLuint shaderMask;
GLuint quadVBO;

GLuint frameCount;
uint64_t m_LastStartTime;
uint64_t m_FrameDuration;
SDL_TimerID animateTimer;
CvCapture * camera;

bool bRunning;

uint32_t videoModeFlags;

SDL_Surface *pDrawContext;

GLuint fbo;
struct Mask{
    GLuint color; 
    GLuint time;
} masks[2];


using namespace std;

int main (int argc, char* argv[]) {

	camera = cvCaptureFromCAM(CV_CAP_ANY);
	
    if (!camera)
        abort();
	
	cvSetCaptureProperty(camera, CV_CAP_PROP_SATURATION, 0);
	cvSetCaptureProperty(camera, CV_CAP_PROP_FPS, 100);
	cvSetCaptureProperty(camera, CV_CAP_PROP_BRIGHTNESS, 0);
    
	capture_frame = cvQueryFrame(camera);
    current_frame = cvCreateImage(cvSize(capture_frame->width, capture_frame->height), IPL_DEPTH_8U, 3);
    cvFlip(capture_frame, current_frame, 1);

    pDrawContext = NULL;
    windowedWidth = WINDOW_WIDTH;
    windowedHeight = WINDOW_HEIGHT;
    videoModeFlags = SDL_OPENGL | SDL_RESIZABLE;
    bRunning = true;
        
    // Initialisation of SDL and creation of OpenGL context
    initSDLOpenGL();
    initHardware(current_frame->width, current_frame->height);
    //cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    loop();
    return 0;
}

void initTimers() {
   // animateTimer = SDL_AddTimer(20, genericTimer, &timerId);
}

void initSDLOpenGL() {
    
    int sdlError = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdlError < 0)
        cout << "Unable to init SDL : " << SDL_GetError() << endl;
    
	//SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Double buffering
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); // Depth buffer size of 16-bit

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

    glClearColor(0., 0., 0., 1.);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    
	
    //glLineWidth(2);
}

void resize(GLuint w, GLuint h) {
    
    width = w;
    height = h;
    
    pDrawContext = SDL_SetVideoMode(width, height, 0, videoModeFlags);

    glViewport(0, 0, width, height);
}

void handleUserEvent(const SDL_Event& event) {  
    if (event.user.code == timerId) {
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
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    capture_frame = cvQueryFrame(camera);
    cvFlip(capture_frame, current_frame, 1);
    
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, camTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, current_frame->width, current_frame->height, 0, GL_RGB, GL_UNSIGNED_BYTE, current_frame->imageData);

	glUseProgram(shaderCompo);
    
	glUniform1i(glGetUniformLocation(shaderCompo, "camTexture"), 0);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    /* The function ! create pointer between vbo datas and shader
     * Manage to use datas 2 by 2 (2 coords for each vertex)
     */
    
    // get the id of the var in shader
    GLuint posLoc = glGetAttribLocation(shaderCompo, "vertPosition");
    GLuint texLoc = glGetAttribLocation(shaderCompo, "textPosition");
    
    glEnableVertexAttribArray(posLoc);
    glEnableVertexAttribArray(texLoc);
    
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    

    
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
        renderFrame();
        while (SDL_PollEvent(&event))
            handleEvent(event); // Checks each event for types and loach corresponding actions
        printGlErrors();
    }
}

GLuint createProgram(char* vertexPath, char* fragmentPath) {
    GLuint vertexShader, fragmentShader;
    
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
	const char* vertexSource = textFileRead(vertexPath);
	const char* fragmentSource = textFileRead(fragmentPath);
    //cout << vertexSource << endl << fragmentSource << endl;
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    
	free((void*)vertexSource);
    free((void*)fragmentSource);
    
    GLuint shaders[2] = {vertexShader, fragmentShader};

    GLint compiled;
    for (int i = 0; i < 2; ++i) {
        glCompileShader(shaders[i]);
    
        glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &compiled); 
    
        // Print compilation error (if any)
        if( compiled != GL_TRUE ){
            
            GLint length; GLchar *buffer;
            glGetShaderiv( shaders[i], GL_INFO_LOG_LENGTH, &length);
            buffer = new char[length];
            glGetShaderInfoLog( shaders[i], length, NULL, buffer);
            std::cerr << buffer << std::endl;
            std::cerr << std::endl << "-------------------------------------" << std::endl;
            delete[] buffer;
        }
    }
    
    GLuint prgm = glCreateProgram();
	glAttachShader(prgm, fragmentShader);
	glAttachShader(prgm, vertexShader);
    
	glLinkProgram(prgm);
    
    GLint linked;
    glGetProgramiv( prgm, GL_LINK_STATUS, &linked);
    
    // Print linking error (if any)
    if( linked != GL_TRUE ){
        
        GLint length; char *buffer;
        glGetProgramiv( prgm, GL_INFO_LOG_LENGTH, &length );
        buffer = new char[length];
        glGetProgramInfoLog( prgm, length, NULL, buffer );
        std::cerr << buffer << std::endl << "-------------------------------------" << std::endl;
        delete[] buffer;
    }
    
    return prgm;
}

void initHardware(const int w, const int h) {
    unsigned short *voidData = new unsigned short[w * h];
    memset(voidData, 0, w * h * sizeof(unsigned short));
    
    for (int i = 0; i < 2; ++i) {
        glGenTextures(1, &masks[i].color);
        glGenTextures(1, &masks[i].time);
        
        glBindTexture(GL_TEXTURE_2D, masks[i].color);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
        glBindTexture(GL_TEXTURE_2D, masks[i].time);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED/*GL_R16I*/, w, h, 0, GL_RED, GL_UNSIGNED_SHORT, voidData);

    }
    
    delete [] voidData;
    
	glBindTexture(GL_TEXTURE_2D, camTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_SHORT, current_frame->imageData);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GLenum fboTargets[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, fboTargets);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float quadCoord[4 * 4] = {-1, 1, 0, 0,
                            -1, -1, 0, 1,
                            1, -1, 1, 1,
                            1, 1, 1, 0};
    
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), quadCoord, GL_STATIC_DRAW);
 
#ifdef __APPLE__
    shaderCompo = createProgram("shaders/compoDarwin.vert", "shaders/compoDarwin.frag");
    shaderMask = createProgram("shaders/maskDarwin.vert", "shaders/maskDarwin.frag");
#else 
    shaderCompo = createProgram("src/shaders/compo.vert", "src/shaders/compo.frag");
    shaderMask = createProgram("src/shaders/mask.vert", "src/shaders/mask.frag");
#endif
}