#include "Recorder.hpp"
#include "OpenGL.hpp"
#include <iostream>

using namespace std;

Recorder::Recorder(const string& path, const size_t& w, const size_t& h) : 
    m_recordPath(path),
    m_width(w),
    m_height(h),
    m_flux(cvCreateVideoWriter(m_recordPath, CV_FOURCC('P','I','M','1'), 25, cvSize(w, h), 1)),
    m_recording(false) {
}

void Recorder::write(const char* data) {
    IplImage writeFrame = cvCreateImage(cvSize(m_width, m_height), IPL_DEPTH_8U, 3);
    writeFrame.imageData = data;
    cvWriteFrame(m_flux, writeFrame);
}

