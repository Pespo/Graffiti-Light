#ifndef __RECORDER_H__
#define __RECORDER_H__

#include "Config.hpp"
#include "Texture.hpp"

class Recorder {
    string m_recordPath;
    CvVideoWriter* m_flux;
    bool m_recording;
    
public:
    Recorder();
    ~Recorder();
};

#endif // __RECORDER_H__