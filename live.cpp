#include "utils.h"

using namespace std;
using namespace cv;

void liveVideoProcessing(int deviceNum, char * size){


    // Video feed
    try{
        VideoCapture cam(deviceNum); // open the default camera
        // Check if camera opened successfully
        if(!cam.isOpened()){
            cout << "Error opening video stream\n" << endl;
            return;
        }

        // Define frame objects
        Mat frame, grey;
        
        // Add a window
        namedWindow("edges",1);

        // Set Video frame dimentions
        //cam.set(CAP_PROP_FRAME_WIDTH, 640);
        //cam.set(CAP_PROP_FRAME_HEIGHT, 480);

        Mat testFrame;
        cam >> testFrame;
        if(cam.get(CAP_PROP_FRAME_WIDTH) != testFrame.cols || cam.get(CAP_PROP_FRAME_HEIGHT) != testFrame.rows){
            cout << "Video feed dimention issues\n";
            return;
        }

        //VideoWriter video("images/JPout.avi",CV_FOURCC('M','J','P','G'),10, Size(1820, 714));

        while(1){
            // Get single frame
            cam >> frame; 

            // Turn into black and white
            cvtColor(frame, grey, COLOR_BGR2GRAY);
            int width = cam.get(CAP_PROP_FRAME_WIDTH);
            int height = cam.get(CAP_PROP_FRAME_HEIGHT);

            //Insert processing here
            vector<int> originalImage(grey.begin<unsigned char>(), grey.end<unsigned char>());

            // These variables will  be updated in the following function
            int xratio, yratio, newWidth, newHeight, asciiWidth, asciiHeight;
            vector<char> reduced = reduceAndMap(originalImage, width, height, newWidth, newHeight, asciiWidth, asciiHeight, xratio, yratio, size);

            // Create vector of pixels based on ascii chars, variables below are updated in the functin
            int finalWidth, finalHeight;
            vector<unsigned char> pixels = asciiToPixels(reduced, asciiWidth, asciiHeight, finalWidth,  finalHeight);

            // Create a frame made out of pixels
            Mat processedFrame (finalWidth, finalHeight, CV_8UC1, pixels.data());

            // Show frame
            imshow("edges", processedFrame);

            // Save to file
            //video.write(mm);
            
            // Some shit that's required to show the frame
            if (waitKey(1) >= 0)
                break;

        } 

    } catch(Exception e){
        cerr << "ERROR(live.cpp): Video device [" << deviceNum << "] cannot be accessed.\n";
        return;
    }

    return;
}