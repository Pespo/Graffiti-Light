#include "functions.hpp"
#include "headers.hpp"
#include <cassert>
#include <iostream>
#include <cstdio>
#include <ctime>

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
	
    while ((current_frame = cvQueryFrame(camera))) {
		
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