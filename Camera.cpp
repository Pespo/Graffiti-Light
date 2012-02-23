#include "Camera.hpp"
#include "OpenGL.hpp"
#include <iostream>

using namespace std;

Camera::Camera() {

}

Camera::Camera(const char* filename) :
    m_flux(cvCaptureFromAVI(filename)),
    m_texture(GL_NEAREST, GL_NEAREST),
    m_currentFrame(cvCreateImage(cvSize(get(WIDTH), get(HEIGHT)), IPL_DEPTH_8U, 3)) {
    cout << "Camera : new (from file)" << endl;
}

Camera::Camera(const int index = CV_CAP_ANY) :
    m_flux(cvCaptureFromCAM(index)),
    m_texture(GL_NEAREST, GL_NEAREST),
    m_currentFrame(cvCreateImage(cvSize(get(WIDTH), get(HEIGHT)), IPL_DEPTH_8U, 3)) {
    cout << "Camera : new (from flux)" << endl;
}

Camera::~Camera() {
    glDeleteBuffers(1, &m_glId);
    //cvReleaseImage(&m_captureFrame);
    //cvReleaseImage(&m_currentFrame);
    cvReleaseCapture(&m_flux);
}

void Camera::set(const Property propId, const double& value) const {
    cvSetCaptureProperty(m_flux, propId, value);
}

double Camera::get(const Property propId) const {
    return cvGetCaptureProperty(m_flux, propId);
}

Texture& Camera::capture() {
    m_captureFrame = cvQueryFrame(m_flux);
    cvFlip(m_captureFrame, m_currentFrame, 1);	
    m_texture.bind();
    m_texture.attachData(m_currentFrame->imageData, get(WIDTH), get(HEIGHT), GL_RGB, GL_UNSIGNED_BYTE);
    m_texture.unbind();
    return m_texture;
}

const Texture& Camera::getTexture() const {
    return m_texture;
}