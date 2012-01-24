// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#ifndef __HEADERS_H__
#define __HEADERS_H__
	
	#include "functions.hpp"

	#if defined _WIN64 || defined _WIN32
		#pragma once
		#include "targetver.h"
		#include <opencv2/opencv.hpp>
		#include <tchar.h>
	#elif __APPLE__
		#include <opencv2/opencv.hpp>
	#elif __linux
		// linux
	#elif __unix // all unices not caught above
		// Unix
	#elif __posix
		// POSIX
	#endif


	#include <cassert>
	#include <iostream>
	#include <cstdio>
	#include <ctime>
	#include <stdio.h>

#endif





// TODO: faites r�f�rence ici aux en-t�tes suppl�mentaires n�cessaires au programme
