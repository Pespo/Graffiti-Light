// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
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





// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme
