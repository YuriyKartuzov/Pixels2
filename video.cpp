#include "utils.h"
#include <omp.h>
#include <stdio.h>

using namespace std;
using namespace cv;

void video(int argc, const char * argv[]){

    // Setting up file names
    const char * filename = argv[2];

    char fileIn[80] = "./input/";
    strcat(fileIn, filename);  
    strcat(fileIn, ".mp4");

    char fileOut[80] = "./output/";
    strcat(fileOut, filename);
    strcat(fileOut, "OUT.mp4");

    cout << "FileIN:" << fileIn << endl;
    cout << "Fileout:" << fileOut << endl;

    // Video feed
    // Access camera and validation
    VideoCapture videoIn(fileIn); 
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
    remove(fileOut);
    VideoWriter videoOut(fileOut, VideoWriter::fourcc('m','j','p','g'), videoIn.get(CAP_PROP_FPS), Size(width, height), 0);  

    // MAIN VIDEO STREAM PROCESSING
    while(1){
        Mat frame, cropped;   
        bool isSuccess = videoIn.read(frame);
        if(!isSuccess)
            break;
            
        // Turn into black and white
        cv::cvtColor(frame, frame, COLOR_BGR2GRAY);

        // Crop the image
        cv::resize(frame, cropped, cv::Size(width, height), 0, 0, INTER_CUBIC);

        //Processing - output is updated
        imageProcessing(cropped.data, output, characters, width, height, fontWidth, fontHeight, numOfChars);

        // Create new frame with output
        cv::Mat processedFrame (height, width, CV_8UC1, output);
        
        // Write to file
        videoOut.write(processedFrame);   
    } 

    videoOut.release();
}






