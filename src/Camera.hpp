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
    
    CvCapture* m_pFlux;
    Texture m_texture;
    
    IplImage* m_pCaptureFrame;
    IplImage* m_pCurrentFrame;
    
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
    IplImage* getCurrentFrame();
    void resize(const size_t& w, const size_t& h);
};

#endif // __CAMERA_H__