#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Config.hpp"
#include "Texture.hpp"

class Camera {
public:
    enum Property {
        WIDTH = CV_CAP_PROP_FRAME_WIDTH,
        HEIGHT = CV_CAP_PROP_FRAME_HEIGHT,
        FPS = CV_CAP_PROP_FPS
    };
    
private:
    GLuint m_glId;
    
    CvCapture* m_flux;
    Texture m_texture;
    
    IplImage* m_captureFrame;
    IplImage* m_currentFrame;
    
public:
    Camera();
    Camera(const char* filename);
    Camera(const int index);    
    ~Camera();
    
    void set(const Property propId, const double& value) const;
    double get(const Property propId) const;
    Texture& capture();
    const Texture& getTexture() const;
    CvCapture* getFlux() const;
};

#endif // __CAMERA_H__