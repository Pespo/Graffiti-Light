#ifndef __HEADERS_H__
#define __HEADERS_H__

#ifdef _WIN64

#elif _WIN32

#else
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

#endif