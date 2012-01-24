
    #include "stdafx.h"


void resetMask(unsigned char *mask, unsigned int width, unsigned int height) {
	for (unsigned int row = 0; row < height; ++row) {
		for (unsigned int col = 0; col < width; ++col) {
			mask[row * 4 * width + col * 4] = 0;
			mask[row * 4 * width + col * 4 + 1] = 0;			
			mask[row * 4 * width + col * 4 + 2] = 0;			
			mask[row * 4 * width + col * 4 + 3] = 0;
		}
	}
}

void writeMask(unsigned char *image, unsigned char *mask, unsigned int seuil, unsigned int width, unsigned int height) {
	for (unsigned int row = 0; row < height; ++row) {
		for (unsigned int col = 0; col < width; ++col) {
			unsigned int rgb = 4 * image[row * 3 * width + col * 3] + 3 * image[row * 3 * width + col * 3 + 1] + 3 * image[row * 3 * width + col * 3 + 2];
			
			if (rgb >= seuil) {
				mask[row * 4 * width + col * 4] = image[row * 3 * width + col * 3];
				mask[row * 4 * width + col * 4 + 1] = image[row * 3 * width + col * 3 + 1];
				mask[row * 4 * width + col * 4 + 2] = image[row * 3 * width + col * 3 + 2];			
				mask[row * 4 * width + col * 4 + 3] = (char)(((double)255 / (double)(2550 - seuil)) * (rgb - seuil));
			}
		}
	}
}

void writeMaster(unsigned char *master, unsigned char *mask, unsigned int width, unsigned int height) {
	for (unsigned int row = 0; row < height; ++row) {
		for (unsigned int col = 0; col < width; ++col) {
			if (master[row * 4 * width + col * 4 + 3] < mask[row * 4 * width + col * 4 + 3]) {
				master[row * 4 * width + col * 4] = mask[row * 4 * width + col * 4];
				master[row * 4 * width + col * 4 + 1] = mask[row * 4 * width + col * 4 + 1];
				master[row * 4 * width + col * 4 + 2] = mask[row * 4 * width + col * 4 + 2];
				master[row * 4 * width + col * 4 + 3] = mask[row * 4 * width + col * 4 + 3];
			}
		}
	}
}

void writeImage(unsigned char *image, unsigned char *master, unsigned int width, unsigned int height) {
	unsigned char aM;
	unsigned char aI;
	unsigned char rM, gM, bM, rI, gI, bI;
	
	for (unsigned int row = 0; row < height; ++row) {
		for (unsigned int col = 0; col < width; ++col) {	
			if (master[row * 4 * width + col * 4 + 3] != 0) {
				rM = master[row * 4 * width + col * 4];
				gM = master[row * 4 * width + col * 4 + 1];
				bM = master[row * 4 * width + col * 4 + 2];
				aM = master[row * 4 * width + col * 4 + 3];
				
				rI = image[row * 3 * width + col * 3];
				gI = image[row * 3 * width + col * 3 + 1];
				bI = image[row * 3 * width + col * 3 + 2];
				aI = 255 - aM;
				
				image[row * 3 * width + col * 3] = (rI * aI + rM * aM) / (aI + aM);
				image[row * 3 * width + col * 3 + 1] = (gI * aI + gM * aM) / (aI + aM);
				image[row * 3 * width + col * 3 + 2] = (bI * aI + bM * aM) / (aI + aM);
			}
		}
	}
}