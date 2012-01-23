#ifndef __HEADERS_H__
#define __HEADERS_H__

#ifdef _WIN64
    //define something for Windows (64-bit)
#elif _WIN32
    //define something for Windows (32-bit)
#elif __APPLE__
    #include <opencv2/opencv.hpp>
#elif __linux
    // linux
#elif __unix // all unices not caught above
    // Unix
#elif __posix
    // POSIX
#endif

#endif