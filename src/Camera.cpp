#include "Camera.hpp"
#include "OpenGL.hpp"
#include <iostream>

using namespace std;

Camera::Camera() {

}

Camera::Camera(const char* filename) :
    m_pFlux(cvCaptureFromAVI(filename)),
    m_texture(GL_NEAREST, GL_NEAREST),
    m_pCurrentFrame(cvCreateImage(cvSize(get(WIDTH), get(HEIGHT)), IPL_DEPTH_8U, 3)) {
        cout << "Camera : new (from file)" << endl;
        cout << "Camera : width = " << m_pCurrentFrame->width <<  endl;
        cout << "Camera : height = " << m_pCurrentFrame->height << endl;
}

Camera::Camera(const int index) :
    m_pFlux(cvCaptureFromCAM(index)),
    m_texture(GL_NEAREST, GL_NEAREST),
    m_pCurrentFrame(cvCreateImage(cvSize(get(WIDTH), get(HEIGHT)), IPL_DEPTH_8U, 3)) {
    cout << "Camera : new (from flux)" << endl;
}

Camera::~Camera() {
    glDeleteBuffers(1, &m_glId);
    //cvReleaseImage(&m_pCaptureFrame);
    //cvReleaseImage(&m_pCurrentFrame);
    cvReleaseCapture(&m_pFlux);
}

void Camera::set(const Property propId, const double& value) const {
    cvSetCaptureProperty(m_pFlux, propId, value);
}

double Camera::get(const Property propId) const {
    return cvGetCaptureProperty(m_pFlux, propId);
}

Texture& Camera::capture() {
    m_pCaptureFrame = cvQueryFrame(m_pFlux);
    cvFlip(m_pCaptureFrame, m_pCurrentFrame, 1);
//    m_pCurrentFrame = cvQueryFrame(m_pFlux);
    m_texture.bind();
    m_texture.attachData(m_pCurrentFrame->imageData, get(WIDTH), get(HEIGHT), GL_RGB, GL_UNSIGNED_BYTE);
    m_texture.unbind();
    return m_texture;
}

const Texture& Camera::getTexture() const {
    return m_texture;
}

CvCapture* Camera::getFlux() const {
    return m_pFlux;
}

void Camera::resize(const size_t& w, const size_t& h) {
    set(WIDTH, w); set(HEIGHT, h);
    m_pCurrentFrame = cvCreateImage(cvSize(get(WIDTH), get(HEIGHT)), IPL_DEPTH_8U, 3);
}

IplImage* Camera::getCurrentFrame() {
    return m_pCurrentFrame;
}