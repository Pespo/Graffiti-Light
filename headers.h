// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
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






// TODO: faites r�f�rence ici aux en-t�tes suppl�mentaires n�cessaires au programme
