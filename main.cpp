#include "functions.hpp"
#include "headers.hpp"
#include "Timer.h"
#include <cassert>
#include <iostream>
#include <cstdio>
#include <ctime>


// GLUT CALLBACK functions ////////////////////////////////////////////////////
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);

// CALLBACK function when exit() called ///////////////////////////////////////
void exitCB();

// function declearations /////////////////////////////////////////////////////
void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void clearSharedMem();
void showFPS();
void draw();
bool checkFramebufferStatus();
void printFramebufferInfo();
std::string convertInternalFormatToString(GLenum format);
std::string getTextureParameters(GLuint id);
std::string getRenderbufferParameters(GLuint id);


// constants
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 300;
const int TEXTURE_WIDTH = 256;
const int TEXTURE_HEIGHT = 256;

// global variables
void *font = GLUT_BITMAP_8_BY_13;
GLuint fboId;                       // ID of FBO
GLuint textureId;                   // ID of texture
GLuint rboId;                       // ID of Renderbuffer object
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
int screenWidth, screenHeight;
bool fboSupported;
bool fboUsed;
int drawMode = 0;
Timer timer, t1;
float playTime;                     // to compute rotation angle
float renderToTextureTime;          // elapsed time for render-to-texture
using namespace std;

int main(int argc, char **argv)
{
    initSharedMem();
    
    // register exit callback
    atexit(exitCB);
    
    // init GLUT and GL
    initGLUT(argc, argv);
    initGL();
    
    // create a texture object
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // get OpenGL info
    /*glInfo glInfo;
    glInfo.getInfo();
    glInfo.printSelf();*/
    
/*#ifdef _WIN32
    // check if FBO is supported by your video card
    if(glInfo.isExtensionSupported("GL_EXT_framebuffer_object"))
    {
        // get pointers to GL functions
        glGenFramebuffersEXT                     = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
        glDeleteFramebuffersEXT                  = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
        glBindFramebufferEXT                     = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
        glCheckFramebufferStatusEXT              = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
        glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
        glGenerateMipmapEXT                      = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");
        glFramebufferTexture2DEXT                = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
        glFramebufferRenderbufferEXT             = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
        glGenRenderbuffersEXT                    = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
        glDeleteRenderbuffersEXT                 = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
        glBindRenderbufferEXT                    = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
        glRenderbufferStorageEXT                 = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
        glGetRenderbufferParameterivEXT          = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
        glIsRenderbufferEXT                      = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
        
        // check once again FBO extension
        if(glGenFramebuffersEXT && glDeleteFramebuffersEXT && glBindFramebufferEXT && glCheckFramebufferStatusEXT &&
           glGetFramebufferAttachmentParameterivEXT && glGenerateMipmapEXT && glFramebufferTexture2DEXT && glFramebufferRenderbufferEXT &&
           glGenRenderbuffersEXT && glDeleteRenderbuffersEXT && glBindRenderbufferEXT && glRenderbufferStorageEXT &&
           glGetRenderbufferParameterivEXT && glIsRenderbufferEXT)
        {
            fboSupported = fboUsed = true;
            cout << "Video card supports GL_EXT_framebuffer_object." << endl;
        }
        else
        {
            fboSupported = fboUsed = false;
            cout << "Video card does NOT support GL_EXT_framebuffer_object." << endl;
        }
    }

#else // for linux, do not need to get function pointers, it is up-to-date*/
    /*if(glInfo.isExtensionSupported("GL_EXT_framebuffer_object"))
    {
        fboSupported = fboUsed = true;
        cout << "Video card supports GL_EXT_framebuffer_object." << endl;
    }
    else
    {
        fboSupported = fboUsed = false;
        cout << "Video card does NOT support GL_EXT_framebuffer_object." << endl;
    }*/
    fboSupported = fboUsed = true;
//#endif
    
    if(fboSupported)
    {
        // create a framebuffer object, you need to delete them when program exits.
        glGenFramebuffersEXT(1, &fboId);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
        
        // create a renderbuffer object to store depth info
        // NOTE: A depth renderable image should be attached the FBO for depth test.
        // If we don't attach a depth renderable image to the FBO, then
        // the rendering output will be corrupted because of missing depth test.
        // If you also need stencil test for your rendering, then you must
        // attach additional image to the stencil attachement point, too.
        glGenRenderbuffersEXT(1, &rboId);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
        
        // attach a texture to FBO color attachement point
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureId, 0);
        
        // attach a renderbuffer to depth attachment point
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, rboId);
        
        //@ disable color buffer if you don't attach any color buffer image,
        //@ for example, rendering depth buffer only to a texture.
        //@ Otherwise, glCheckFramebufferStatusEXT will not be complete.
        //glDrawBuffer(GL_NONE);
        //glReadBuffer(GL_NONE);
        
        // check FBO status
        //printFramebufferInfo();
        bool status = checkFramebufferStatus();
        if(!status)
            fboUsed = false;
        
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }
    
    // start timer, the elapsed time will be used for rotating the teapot
    timer.start();
    
    // the last GLUT call (LOOP)
    // window will be shown and display callback is triggered by events
    // NOTE: this call never return main().
    glutMainLoop(); /* Start GLUT event-processing loop */
    
    return 0;
}

void draw() {
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glBegin(GL_QUADS);
    glColor4f(1, 1, 1, 1);
    
    //glNormal3f(0,0,1);
    glTexCoord2f(1, 1);  glVertex3f(1, 1, 0);
    glTexCoord2f(0, 1);  glVertex3f(-1, 1, 0);
    glTexCoord2f(0, 0);  glVertex3f(-1, -1, 0);
    glTexCoord2f(1, 0);  glVertex3f(1, -1, 0);
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

int initGLUT(int argc, char **argv) {
    // GLUT stuff for windowing
    // initialization openGL window.
    // It must be called before any other GLUT routine.
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA);   // display mode
    
    /*
     * @todo define screenWidth, screenHeight
     */
    glutInitWindowSize(screenWidth, screenHeight);              // window size
    
    glutInitWindowPosition(100, 100);                           // window location
    
    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // It returns a unique ID.
    int handle = glutCreateWindow(argv[0]);     // param is the title of window
    
    // register GLUT callback functions
    glutDisplayFunc(displayCB);
    //glutTimerFunc(33, timerCB, 33);             // redraw only every given millisec
    glutIdleFunc(idleCB);                       // redraw whenever system is idle
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);
    //glutMouseFunc(mouseCB);
    //glutMotionFunc(mouseMotionCB);
    
    return handle;
}

void initGL() {
    /*
     * @check maybe a lot of useful method ?
     */
    //glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment
    
    // enable /disable features
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_CULL_FACE);
    
    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL);
    
    glClearColor(0, 0, 0, 0);                   // background color
    //glClearStencil(0);                          // clear stencil buffer
    //glClearDepth(1.0f);                         // 0 is near, 1 is far
    //glDepthFunc(GL_LEQUAL);
    
    //initLights();
    //setCamera(0, 0, 6, 0, 0, 0);
}

bool initSharedMem() {
    /*
     * @todo Create constants
     */
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;
    //mouseLeftDown = mouseRightDown = false;
    //cameraAngleX = cameraAngleY = 45;
    //cameraDistance = 0;
    //playTime = 0;
    return true;
}

void clearSharedMem() {
    glDeleteTextures(1, &textureId);
    
    // clean up FBO, RBO
    if(fboSupported)
    {
        glDeleteFramebuffersEXT(1, &fboId);
        glDeleteRenderbuffersEXT(1, &rboId);
    }
}

bool checkFramebufferStatus() {
    // check FBO status
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    switch(status)
    {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            std::cout << "Framebuffer complete." << std::endl;
            return true;
            
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
            return false;
            
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
            return false;
            
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
            return false;
            
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
            return false;
            
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
            return false;
            
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
            return false;
            
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            std::cout << "[ERROR] Unsupported by FBO implementation." << std::endl;
            return false;
            
        default:
            std::cout << "[ERROR] Unknow error." << std::endl;
            return false;
    }
}

void drawScene() {
    glBegin(GL_TRIANGLES);
    glColor4f(1, 0, 0, 1);
    
    //glNormal3f(0,0,1);
    glVertex3f(0, 1, 0);
    glVertex3f(1, -1, 0);
    glVertex3f(-1, -1, 0);
    
    glEnd();
}

//=============================================================================
// CALLBACKS
//=============================================================================

void displayCB()
{
    // get the total elapsed time
    playTime = (float)timer.getElapsedTime();
    
    // compute rotation angle
    //const float ANGLE_SPEED = 90;   // degree/s
    //float angle = ANGLE_SPEED * playTime;
    
    // render to texture //////////////////////////////////////////////////////
    t1.start();
    
    // adjust viewport and projection matrix to texture dimension
    glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60.0f, (float)(TEXTURE_WIDTH)/TEXTURE_HEIGHT, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    
    // with FBO
    // render directly to a texture
    if(fboUsed)
    {
        // set the rendering destination to FBO
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
        
        // clear buffer
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // draw a rotating teapot
        glPushMatrix();
        //glRotatef(angle*0.5f, 1, 0, 0);
        //glRotatef(angle, 0, 1, 0);
        //glRotatef(angle*0.7f, 0, 0, 1);
        //glTranslatef(0, -1.575f, 0);
        drawScene();
        glPopMatrix();
        
        // back to normal window-system-provided framebuffer
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // unbind
        
        // trigger mipmaps generation explicitly
        // NOTE: If GL_GENERATE_MIPMAP is set to GL_TRUE, then glCopyTexSubImage2D()
        // triggers mipmap generation automatically. However, the texture attached
        // onto a FBO should generate mipmaps manually via glGenerateMipmapEXT().
        glBindTexture(GL_TEXTURE_2D, textureId);
        glGenerateMipmapEXT(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    /*
    // without FBO
    // render to the backbuffer and copy the backbuffer to a texture
    else
    {
        // clear buffer
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glPushAttrib(GL_COLOR_BUFFER_BIT | GL_PIXEL_MODE_BIT); // for GL_DRAW_BUFFER and GL_READ_BUFFER
        glDrawBuffer(GL_BACK);
        glReadBuffer(GL_BACK);
        
        // draw a rotating teapot
        glPushMatrix();
        glRotatef(angle*0.5f, 1, 0, 0);
        glRotatef(angle, 0, 1, 0);
        glRotatef(angle*0.7f, 0, 0, 1);
        glTranslatef(0, -1.575f, 0);
        drawTeapot();
        glPopMatrix();
        
        // copy the framebuffer pixels to a texture
        glBindTexture(GL_TEXTURE_2D, textureId);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glPopAttrib(); // GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
    }*/
    
    // measure the elapsed time of render-to-texture
    t1.stop();
    renderToTextureTime = t1.getElapsedTimeInMilliSec();
    ///////////////////////////////////////////////////////////////////////////
    
    
    // rendering as normal ////////////////////////////////////////////////////
    
    // back to normal viewport and projection matrix
    glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60.0f, (float)(screenWidth)/screenHeight, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    
    // clear framebuffer
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    glPushMatrix();
    
    // tramsform camera
    //glTranslatef(0, 0, cameraDistance);
    //glRotatef(cameraAngleX, 1, 0, 0);   // pitch
    //glRotatef(cameraAngleY, 0, 1, 0);   // heading
    
    // draw a cube with the dynamic texture
    draw();
    
    glPopMatrix();
    
    // draw info messages
    //showInfo();
    //showFPS();
    glutSwapBuffers();
}

void reshapeCB(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    
    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60.0f, (float)(width)/height, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip
    
    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
}


void timerCB(int millisec)
{
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}


void idleCB()
{
    glutPostRedisplay();
}


void keyboardCB(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: // ESCAPE
            exit(0);
            break;
            
        case ' ':
            if(fboSupported)
                fboUsed = !fboUsed;
            cout << "FBO mode: " << (fboUsed ? "on" : "off") << endl;
            break;
            
        case 'd': // switch rendering modes (fill -> wire -> point)
        case 'D':
            drawMode = drawMode + drawMode % 3;
            if(drawMode == 0)        // fill mode
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_CULL_FACE);
            }
            else if(drawMode == 1)  // wireframe mode
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_CULL_FACE);
            }
            else                    // point mode
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_CULL_FACE);
            }
            break;
            
        default:
            ;
    }
}

void exitCB()
{
    clearSharedMem();
}