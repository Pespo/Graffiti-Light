#ifndef __TIMER_H__
#define __TIMER_H__
#include "Config.hpp"

#ifdef LM_WINDOWS
#include <windows.h>
#else          
#include <sys/time.h>
#endif
#include <cstdlib>

class Timer {
    double startTimeInMicroSec;
    double endTimeInMicroSec;
    int stopped;
#ifdef LM_WINDOWS
    LARGE_INTEGER frequency;
    LARGE_INTEGER startCount;
    LARGE_INTEGER endCount;
#else
    timeval startCount;
    timeval endCount;
#endif
    
public:
    Timer();
    ~Timer();
    void start();
    void stop();
    double getElapsedTime();
    double getElapsedTimeInSec();
    double getElapsedTimeInMilliSec();
    double getElapsedTimeInMicroSec();
};

#endif // __TIMER_H__
