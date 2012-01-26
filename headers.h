// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

	
	#include "functions.hpp"

	#if defined _WIN64 || defined _WIN32
		#pragma once
		#include "targetver.h"
		#include <windows.h> 
		#include <opencv2/opencv.hpp>
		#include <tchar.h>
		#include <stdlib.h>
		#include <stdio.h>
		#include <SDL/SDL.h>
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






// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme
