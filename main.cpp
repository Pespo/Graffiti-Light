#include "headers.hpp"
#include "textfile.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstring>
#include <fstream>

void init();
void initFBOs();
void resize(GLuint w, GLuint h);
void handleUserEvent(const SDL_Event& event);
void handleKeyEvent(const SDL_keysym& keysym, bool down);
void animate();
void handleEvent(const SDL_Event& event);
void renderFrame();
void renderOffScreen();
void renderOnScreen();
void printGlErrors();
void loop();
void initHardware(const int w, const int h);

IplImage * current_frame = NULL;
IplImage * capture_frame = NULL;

size_t screenWidth;
size_t screenHeight;
size_t windowWidth;
size_t windowHeight;
size_t cameraWidth;
size_t cameraHeight;
static int timerId = 1;
GLuint camTexture;
GLuint pingpongId;

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

    pingpongId = 0;
    
    pDrawContext = NULL;
    //cameraWidth = capture_frame->width;
    //cameraHeight = capture_frame->height;
    videoModeFlags = SDL_OPENGL | SDL_RESIZABLE;
    bRunning = true;
    // Initialisation of SDL and creation of OpenGL context
    init();
    initHardware(current_frame->width, current_frame->height);
    //cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    loop();
    return 0;
}

void init() {
    
    int sdlError = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdlError < 0)
        cout << "Unable to init SDL : " << SDL_GetError() << endl;
    
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
    screenWidth = videoInfo->current_w - 100;
    screenHeight = videoInfo->current_h - 100;
    
    camera = cvCaptureFromCAM(CV_CAP_ANY);
	//camera = cvCaptureFromAVI("/Users/Tom/Desktop/test.avi");
    
    if (!camera)
        abort();
    
	cvSetCaptureProperty(camera, CV_CAP_PROP_SATURATION, 0);
	cvSetCaptureProperty(camera, CV_CAP_PROP_FPS, 100);
	cvSetCaptureProperty(camera, CV_CAP_PROP_POS_FRAMES, 10);
    
    cameraWidth = cvGetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH);
    cameraHeight = cvGetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT);
    
    cout << cameraWidth << " - " << cameraHeight << endl;
    
    /*if (cameraHeight > screenHeight || cameraWidth > screenWidth) {
        float widthScale = cameraWidth / screenWidth;
        float heightScale = cameraHeight / screenHeight;
        float rapport = cameraHeight / widthScale;
        
        if (rapport < screenHeight) {
            windowWidth = screenWidth;
            windowHeight = rapport;
        } else {
            windowWidth = cameraWidth / heightScale;
            windowHeight = screenHeight;
        }
    } else {
        windowWidth = cameraWidth;
        windowHeight = cameraHeight;
    }*/
    
       
    float camRap = float(cameraWidth) / float(cameraHeight);
    float screenRap = float(screenWidth) / float(screenHeight);
    
    if (camRap > screenRap) {
        if (cameraWidth < screenWidth) {
            windowWidth = cameraWidth;
			windowHeight = cameraHeight;
        } else {
            windowWidth = screenWidth;
			windowHeight = windowWidth * cameraHeight / cameraWidth;
        }
    } else {
        if (cameraHeight < screenHeight) {
            windowWidth = cameraWidth;
			windowHeight = cameraHeight;
        } else {
            windowWidth = screenHeight * camRap;
			windowHeight = screenHeight;
        }
    }
    
    cout << windowWidth << " - " << windowHeight << endl;    
    
    cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_WIDTH, windowWidth);
    cvSetCaptureProperty(camera, CV_CAP_PROP_FRAME_HEIGHT, windowHeight);
    
    /*
     * Flip the IplImages for deinterleaving
     */

    capture_frame = cvQueryFrame(camera);
    if (!capture_frame)
        abort();
    current_frame = cvCreateImage(cvSize(capture_frame->width, capture_frame->height), IPL_DEPTH_8U, 3);
    cvFlip(capture_frame, current_frame, 1);	

    pDrawContext = SDL_SetVideoMode(windowWidth, windowHeight, 0, videoModeFlags);
    
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
        cout << "GLEW Error : " << glewGetErrorString(glewError) << endl;
    
    glClearColor(0., 0., 0., 1.);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
}

void handleUserEvent(const SDL_Event& event) {  
    if (event.user.code == timerId) {
        renderFrame();
        
        printGlErrors();
    }
}

void handleKeyEvent(const SDL_keysym& keysym, bool down) {
    if (down) {
        switch (keysym.sym) {
            case SDLK_ESCAPE :
                bRunning = false;
                break;                
            case SDLK_r : {
                unsigned char *voidData4B = new unsigned char[4 * current_frame->width * current_frame->height];
                memset(voidData4B, 0, 4 * current_frame->width * current_frame->height * sizeof(unsigned char));
                glBindTexture(GL_TEXTURE_2D, masks[pingpongId].color);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, current_frame->width, current_frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, voidData4B);
                delete [] voidData4B;
                break;
            }
            default :
                break;
        }
    }
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

void buildDrawSurface() {
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    // Get the index of the attribute vars in shader
    GLuint posLoc = glGetAttribLocation(shaderCompo, "vertPosition");
    GLuint texLoc = glGetAttribLocation(shaderCompo, "textPosition");
    
    /* THE function ! create pointer between vbo datas and shader
     * 
     */
    glEnableVertexAttribArray(posLoc);
    glEnableVertexAttribArray(texLoc);
    
    // Define vertex position : 2 values in a range of 4, starting at index 0
    glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    // Define texture position : 2 values in a range of 4, starting at index 2
    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
    
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void renderOffScreen() {
    
    glUseProgram(shaderMask);
    
    // Switch to offScreen fbo
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, masks[pingpongId].color);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, masks[pingpongId].time);
    
    // Switch to destination textures
    pingpongId = pingpongId == 0 ? 1 : 0;
    
    // Attach destination textures to fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, masks[pingpongId].color, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, masks[pingpongId].time, 0);
    
    /* Activate texture at Index 0
     * Draw the frame captured
     */
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, camTexture);
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, masks[pingpongId].color);
    
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, masks[pingpongId].time);
    
    /* Get the index of var "CamTexture" in the shader
     * Set this var to 0 (The index of the texture)
     */
	glUniform1i(glGetUniformLocation(shaderMask, "camTexture"), 0);
    glUniform1i(glGetUniformLocation(shaderMask, "pingColorTexture"), 1);
    glUniform1i(glGetUniformLocation(shaderMask, "pingTimeTexture"), 2);
    
    buildDrawSurface();
}

void renderOnScreen() {
    

    glUseProgram(shaderCompo);
    
    // Point to the screen again
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, camTexture);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, masks[pingpongId].color);
    
//    //====================
//    // HACK
//    //====================
//    
//    unsigned char data[640 * 426 * 4];
//    
//    for (int h = 0; h < 426; ++h) {
//        for (int w = 0; w < 640 * 4; w += 4) {
//            data[h * 640 * 4 + w] = 0;
//            data[h * 640 * 4 + w + 1] = 0;
//            data[h * 640 * 4 + w + 2] = 0;
//            if (h < 200) {
//                data[h * 640 * 4 + w + 3] = 125;
//            } else {
//                data[h * 640 * 4 + w + 3] = 0;
//            }
//            
//            
//        }
//    }
//    //memset(data, 255, 4 * 640 * 426 * sizeof(unsigned char));
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 426, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

//    //====================
//    // HACK
//    //====================
    
    glUniform1i(glGetUniformLocation(shaderCompo, "camTexture"), 0);
    glUniform1i(glGetUniformLocation(shaderCompo, "maskTexture"), 1);
    
    buildDrawSurface();
}

void renderFrame() {
    capture_frame = cvQueryFrame(camera);
    cvFlip(capture_frame, current_frame, 1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, camTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, current_frame->width, current_frame->height, 0, GL_RGB, GL_UNSIGNED_BYTE, current_frame->imageData);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderOffScreen();
    
    renderOnScreen();
    
    SDL_GL_SwapBuffers();
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

void initFBO() {
    /* FBO for send the mask to the gpu
     * 
     */
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // Two targets in the FBO : one color texture, one time texture
    GLenum fboTargets[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, fboTargets);
    
    // Point to the screen again
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void initTextures(const int w, const int h) {

    unsigned short *voidData1B = new unsigned short[w * h];
    memset(voidData1B, 1, w * h * sizeof(unsigned short));
    
    unsigned char *voidData4B = new unsigned char[4 * w * h];
    memset(voidData4B, 1, 4 * w * h * sizeof(unsigned char));
    
    /* Two masks for ping ponging
     */
    for (int i = 0; i < 2; ++i) {
        /* Generate textures for the mask
         *  One for the print
         *  One for the life time
         */
        glGenTextures(1, &masks[i].color);
        glGenTextures(1, &masks[i].time);
        
        /* Bind the texture for drawing 
         * Paramtering
         * It's not initialized because it will be done at each frame
         */
        glBindTexture(GL_TEXTURE_2D, masks[i].color);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, voidData4B);

        /* Bind the texture for drawing 
         * Parametering
         * Initialized with zeros
         */
        glBindTexture(GL_TEXTURE_2D, masks[i].time);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_SHORT, voidData1B);
    }
    
    delete [] voidData1B;
    delete [] voidData4B;
    
    /* Texture for saving the camera frames
     * Initialisation with one frame
     */
    glGenTextures(1, &camTexture);
	glBindTexture(GL_TEXTURE_2D, camTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_SHORT, current_frame->imageData);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void initDrawSurface() {
    /* Coords for one quad in all screen
     * Two coords for the vertice
     * Two coords for the texture
     */
    float quadCoord[4 * 4] = {-1, 1, 0, 0,
        -1, -1, 0, 1,
        1, -1, 1, 1,
        1, 1, 1, 0};
    
    /* Create Vertex Buffer Object to stock the quad
     * GL_STATIC_DRAW : fastest drawing - calculated one time
     */
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), quadCoord, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initShaders() {
#ifdef __APPLE__
    shaderCompo = createProgram("shaders/compoDarwin.vert", "shaders/compoDarwin.frag");
    shaderMask = createProgram("shaders/maskDarwin.vert", "shaders/maskDarwin.frag");
#else 
	shaderCompo = createProgram("src/shaders/compoDarwin.vert", "src/shaders/compoDarwin.frag");
    shaderMask = createProgram("src/shaders/maskDarwin.vert", "src/shaders/maskDarwin.frag");
    //shaderCompo = createProgram("src/shaders/compo.vert", "src/shaders/compo.frag");
    //shaderMask = createProgram("src/shaders/mask.vert", "src/shaders/mask.frag");
#endif*/
}

void initHardware(const int w, const int h) {

    initTextures(w, h);

    initFBO();

    initDrawSurface();
    
    initShaders();
}