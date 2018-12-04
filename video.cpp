#include "utils.h"
#include <omp.h>
#include <stdio.h>

using namespace std;
using namespace cv;
using namespace chrono;

void video(int argc, const char * argv[]){

    // Setting up file names
    const char * filename = argv[2];
    string fileIN = "./input/";
    fileIN += argv[2];
    fileIN += ".mp4";
    
    string fileOUT = "./output/";
    fileOUT += argv[2];
    fileOUT += "OUT.mp4";

    // char fileIn[80] = "./input/";
    // strcat_s(fileIn, filename);  
    // strcat_s(fileIn, ".mp4");

    // char fileOut[80] = "./output/";
    // strcat_s(fileOut, filename);
    // strcat_s(fileOut, "OUT.mp4");

    // Video feeds from camera and validation
    VideoCapture videoIn(fileIN); 
    if(!videoIn.isOpened()){
        cout << "Error opening video file\n" << endl;
        return;
    }

    // Define frame objects
    int originalWidth = videoIn.get(CAP_PROP_FRAME_WIDTH);
    int originalHeight = videoIn.get(CAP_PROP_FRAME_HEIGHT);
    int width, height;
    
    // Reading char files and creating matrix of ascii pixels, and updating fontWidth and fontHeight vars below
    int fontWidth;
    int fontHeight;
    Mat *characters = new Mat[14];
    int numOfChars = loadCharacters(characters, fontWidth, fontHeight);

    // Calculating new width and height for frame resizing
    if(originalWidth % fontWidth == 0)
        width = originalWidth;
    else
        width = int(originalWidth / fontWidth) * fontWidth;
    
    if(originalHeight % fontHeight == 0)
        height = originalHeight;
    else
        height = int(originalHeight / fontHeight) * fontHeight;

    // Output frame
    unsigned char * output = new unsigned char[width * height];
    
    // Video writer
    remove(fileOUT.c_str());
    VideoWriter videoOut(fileOUT, VideoWriter::fourcc('m','p','4','v'), videoIn.get(CAP_PROP_FPS), Size(width, height), 0);

	// REPORT
	printf("Pixels 2.0 program: VIDEO Processing.\n");

//#pragma omp single
	{
		int c = 0;
		int cc = 0;
		while (1) {
			c++;
			cc++;
			Mat frame, cropped, bw;
			bool isSuccess = videoIn.read(frame);
			if (!isSuccess)
				break;

			// Turn into black and white
			//auto start = high_resolution_clock::now();
			cv::cvtColor(frame, bw, COLOR_BGR2GRAY);
			/*f (c % 30 == 0) {
				auto micro = duration_cast<microseconds>(high_resolution_clock::now() - start).count();
				cout << "FRAME: " << cc << endl;
				cout << "OpenCV turned Color to Greyscale in : " << micro << " micro - seconds\n";
			}*/

			// Crop the image
			//start = high_resolution_clock::now();
			//cv::resize(bw, cropped, cv::Size(width, height), 0, 0, INTER_CUBIC); COMPARED 500 vs 200 microsecs for mine
			unsigned char * croppedImage = cropImage(bw.data, originalWidth, originalHeight, width, height);
			/*if (c % 30 == 0) {
				auto micro = duration_cast<microseconds>(high_resolution_clock::now() - start).count();
				cout << "My cropping version did it in: " << micro << " micro-seconds\n";
			}*/

			//start = high_resolution_clock::now();
			//Processing - output is updated. TIME THIS FUNCTION
			imageProcessing(croppedImage, output, characters, width, height, fontWidth, fontHeight, numOfChars);
			/*if (c % 30 == 0) {
				auto micro = duration_cast<microseconds>(high_resolution_clock::now() - start).count();
				cout << "Frame processed in: " << micro << " micro-seconds\n\n";
				c = 0;
			}*/

			// Create new frame with output
			cv::Mat processedFrame(height, width, CV_8UC1, output);

			// Write to file
			videoOut.write(processedFrame);
		}

	}
    videoOut.release();
}






