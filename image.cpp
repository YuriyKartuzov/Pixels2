#include "utils.h"

using namespace std;
using namespace cv;

void imageProcessing(const char * fn, char * size){

    // File names
    char fileIn[80];
    char fileOut[80];
    strcpy(fileIn, fn);
    strcpy(fileOut, fn);
    strcat(fileIn, ".png");
    strcat(fileOut, ".txt");

    try{

        Mat im = imread(fileIn, CV_LOAD_IMAGE_GRAYSCALE);
        if(!im.data){
            cerr << "ERROR: file [" << fileIn << "] could not be read\n";
            return;
        }

        int width = im.cols;
        int height = im.rows;

        // COPY image data to a svector 
        vector<int> originalImage(im.begin<unsigned char>(), im.end<unsigned char>());

        // Map function. These variables will be updated in the following function
        int xratio, yratio, newWidth, newHeight, asciiWidth, asciiHeight;
        vector<char> ascii = reduceAndMap(originalImage, width, height, newWidth, newHeight, asciiWidth, asciiHeight, xratio, yratio, size);

        // Write to file
        std::ofstream myfile (fileOut);
        if(myfile.is_open())
        {
            for(int i=1;i<ascii.size(); i++){
                myfile << ascii.at(i);
                if (i % (asciiWidth) == 0)
                    myfile << "\n";
            }
            cout << "Input file: " << fileIn << endl;
            cout << "Output file: " << fileOut << endl;
        }else
            std::cout << "Write to file failed\n";

    }catch(Exception e){
        cout << "ERROR: " << e.what() << "\n";
    }
    return ;

}