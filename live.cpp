#include "utils.h"
#include <omp.h>

using namespace std;
using namespace cv;

void live(int argc, const char * argv[]){
    // Setting up camera identified
    int deviceNum = 0;
    int frameWidth = 640;
    int frameHeight = 480;

    if(argc == 3)
        deviceNum = atoi(argv[2]);

    if(argc == 4 && !strcmp(argv[3], "large")){
        frameWidth = 1280;
        frameHeight = 780;
        deviceNum = atoi(argv[2]);
    }

    // Video feed, Access camera and validation
    VideoCapture cam(deviceNum); 
    if(!cam.isOpened()){
        cout << "Error opening video stream\n" << endl;
        return;
    }

    // Set frame dimentions
    cam.set(CAP_PROP_FRAME_WIDTH, frameWidth);
    cam.set(CAP_PROP_FRAME_HEIGHT, frameHeight);

    //Check frame dimentions set by openCV
    int originalWidth = cam.get(CAP_PROP_FRAME_WIDTH);
    int originalHeight = cam.get(CAP_PROP_FRAME_HEIGHT);
    int width, height;
    
    cout << "frame width " << frameWidth << ", frame height " << frameHeight << endl;
    cout << "original width " << originalWidth << ", original height " << originalHeight << endl;
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
    //remove("liveout.mp4");
    //cv::namedWindow("Live shit", cv::WINDOW_AUTOSIZE);
    //VideoWriter videoOut("liveout.mp4", CV_FOURCC('m','j','p','g'), 25, Size(width, height), 0); 


    // MAIN VIDEO STREAM PROCESSING
    while(1){
        Mat frame, cropped;   
        bool isSuccess = cam.read(frame);
        if(!isSuccess)
            break;
            
        // Turn into black and white
        cv::cvtColor(frame, frame, COLOR_BGR2GRAY);

        // Crop the image
        cv::resize(frame, cropped, cv::Size(width, height), 0, 0, INTER_CUBIC);

        //Processing - output is updated. TIME THIS FUNCTION
        imageProcessing(cropped.data, output, characters, width, height, fontWidth, fontHeight, numOfChars);

        // Create new frame with output
        cv::Mat processedFrame (height, width, CV_8UC1, output);
        
        // Show frame
        cv::imshow("Live Stream Processing", processedFrame);
        //videoOut.write(processedFrame);

        // Escape
        if (waitKey(10) == 27)
            break;
    } 

    //video.release();
}






