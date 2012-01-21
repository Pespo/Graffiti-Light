#include <OpenCV/OpenCV.h>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <ctime>

void resetMask(unsigned char *mask, unsigned int width, unsigned int height);
void writeMask(unsigned char *image, unsigned char *mask, unsigned int seuil, unsigned int width, unsigned int height);
void writeMaster(unsigned char *master, unsigned char *mask, unsigned int width, unsigned int height);
void writeImage(unsigned char *image, unsigned char *master, unsigned int width, unsigned int height);

const char * WINDOW_NAME = "Graffiti Light";
int seuil = 2550;

using namespace std;

int main (int argc, char * const argv[]) {
	
    cvNamedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    CvCapture * camera = cvCaptureFromCAM(CV_CAP_ANY);
	
    if (!camera)
        abort();
	
	cvSetCaptureProperty(camera, CV_CAP_PROP_SATURATION, 0);
	
    IplImage * current_frame = cvQueryFrame(camera);
    IplImage * draw_image = cvCreateImage(cvSize(current_frame->width, current_frame->height), IPL_DEPTH_8U, 3);
    assert(current_frame && draw_image);
    
	unsigned char *image = new unsigned char[current_frame->width * current_frame->height * 3];
	unsigned char *mask = new unsigned char[current_frame->width * current_frame->height * 4];
	unsigned char *master = new unsigned char[current_frame->width * current_frame->height * 4];	

	cvCvtColor(current_frame, current_frame, CV_BGR2RGB);
	cvCvtColor(draw_image, draw_image, CV_BGR2RGB);
	
	int record = 0;
	CvVideoWriter * writer = 0;

	resetMask(master, draw_image->width, draw_image->height);
	
    while (current_frame = cvQueryFrame(camera)) {
		
        cvFlip(current_frame, draw_image, 1);
        
		resetMask(mask, draw_image->width, draw_image->height);
		writeMask((unsigned char*)draw_image->imageData, mask, seuil, draw_image->width, draw_image->height);
		writeMaster(master, mask, draw_image->width, draw_image->height);
		writeImage((unsigned char*)draw_image->imageData, master, draw_image->width, draw_image->height);
		
        cvShowImage (WINDOW_NAME, draw_image);
		
		if (record) {
			if (writer == 0) {
				writer = cvCreateVideoWriter("/Users/tomgay/Documents/Dropbox/IMAC2/Graffiti Light/Capture/out.avi", CV_FOURCC('P','I','M','1'), 25, cvSize(current_frame->width,current_frame->height), 1);
			}
			cvWriteFrame(writer, draw_image);
		}
		
		int key = cvWaitKey (100);
		if (key == 'v' || key == 'V') {
			record = record == 0 ? 1 : 1;
		}
		if (key == 'r' || key == 'R') {
			resetMask(master, draw_image->width, draw_image->height);
		}
		if (key == 'p' || key == 'P') {
			if (seuil <= 2500)
				seuil += 50;
			std::cout << "seuil : " << seuil << std::endl;
		}
		if (key == 'm' || key == 'M') {
			if (seuil >= 50)
				seuil -= 50;
			std::cout << "seuil : " << seuil << std::endl;
		}
        if (key == 'q' || key == 'Q') {
			cvReleaseImage(&current_frame);
			cvReleaseImage(&draw_image);
			//cvReleaseCapture(&camera);
			delete [] image;
			delete [] mask;
			delete [] master;
			cvReleaseVideoWriter(&writer);
			cvDestroyWindow(WINDOW_NAME);
            break;
		}
    }
    
    return 0;
}

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