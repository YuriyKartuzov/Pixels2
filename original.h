// GPU621 Project
// Alex Kong, Dmytro Sych, Yuriy Kartuzov

#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include "loadpng.h"
#include <algorithm>
#include <iterator>

 #include "opencv2/core/core.hpp"

using namespace cv;
 
 
// Prototypes
std::vector<int> MakeBW(std::vector<unsigned char> image);
std::vector<int> CropImage(const std::vector<int> bwImage, int width, int height, int newWidth, int newHeight);
std::vector<char> ReduceAndMap(const std::vector<int> image, int newImageWidth, int newImageHeight, int xratio, int yratio);
char pixelToChar(int pixel, bool reverse);
void WriteToFile(std::vector<char> image, int imageWidth, const char* filename);


int main(int argc, const char * argv[]) {
    
    //Filename
    const char* filename = "r2d2";
    char str[80];
    strcpy(str, "images/");
    strcat(str, filename);
    strcat(str, ".png");
    
    // 1. Original image capture
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, str);
    if(error) {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << "\n\n";
        return 0;
    }
    if(width < 800 || height < 500){
        std::cout << "The file of too small of a resolution, please upload a bigger file";
        return 0;
    }

    // 2. Make BW
    std::vector<int> BWimage =  MakeBW(image);

    
    // 3. Dynamic selection of Xration and Yratio;
    int xratio;
    int yratio;

    if( (width / height) < 0){
        yratio = int(floor(width / 180));
        xratio = int(yratio / 2);
    } else {
        xratio = int(floor(height / 256));
        yratio = int(xratio * 2);
    }
    
    int newWidth = (xratio * (int)(width / xratio));
    int newHeight = (yratio * (int)(height / yratio));
    
    
    // 4. Crop image
    std::vector<int> croppedImage = CropImage( BWimage,  width,  height,  newWidth,  newHeight);
   
    // 5. Reduce and Map image to CHAR
    std::vector<char> asciiImage = ReduceAndMap(croppedImage, newWidth, newHeight, xratio, yratio);


    // 6. Drawing in a file - Main output
    WriteToFile(asciiImage, newWidth / xratio, filename);
    
    // Reporting
    printf(" INPUT file: %s.png\n OUTPUT file: %s.txt\n\n", filename, filename);
    printf(" ORIGINAL     file width: %d, height %d, pixels: %d\n", width, height, (int)image.size() / 4); //4 refers TO RBGA
    //printf(" PREPPED      file width: %d, height %d, pixels: %d\n", newWidth, newHeight, (int)cropeedImage.size() );
    printf(" REDUCED text file width: %d, height %d, chars: %d\n\n", newWidth / xratio, newHeight / yratio, (int)asciiImage.size() );

    return 0;
}



// Getting a single Luminocity out of RGBA set. Using standard formula (0.2126*R + 0.7152*G + 0.0722*B)
std::vector<int> MakeBW(std::vector<unsigned char> image){
    std::vector<int> bwImage;
    
    for(int i=0; i<image.size(); i+=4){
        bwImage.push_back(
                          (int)(0.2126 * (int)image.at(i)) + (0.7152 * (int)image.at(i+1)) + (0.0722 * (int)image.at(i+2))
                          );
    }
    return bwImage;
}


// Cropping an Image so that width is multiple of 4 and height of 7
std::vector<int> CropImage(const std::vector<int> bwImage, int width, int height, int newWidth, int newHeight){
    std::vector<int> croppedImage;
    std::vector<int>::const_iterator current = bwImage.begin();
    
    for(int i = 0 ; i < height; i++){
        std::copy(current, current + newWidth, std::back_inserter(croppedImage));
        current += width;
    }
    
    return croppedImage;
}


// Reducing Image
std::vector<char> ReduceAndMap(const std::vector<int> image, int width, int height, int xratio, int yratio){
    
    std::vector<char> mappedImage;
    int xP = 0; // Main index for treversing the image
    int sum = 0;
    
    // Row block treversal
    for(int y = 0; xP < image.size() - (yratio * width); y++){
        
        // Colomn block treversal
        for(int x = 0; x < (width / xratio); x++){
            
            // Rows treversal
            for(int i = 0; i < yratio; i++){
                
                // Colomn treversal
                for(int j = 0; j < xratio; j++){
                    sum += image.at(xP + j);
                }
                
                xP += width;
            }
            
            // Getting average and mapping to CHAR
            int average = (int)sum / (xratio * yratio);
            mappedImage.push_back( pixelToChar(average, false) );
            
            // reposition & reset
            xP = xP - (yratio * width) + xratio; // ход конем
            sum = 0;
        }
        
        xP += (yratio * width);
    }
    return mappedImage;
}


// Mapping to ASCII
char pixelToChar(int pixel, bool reverse) {
    // Original 70 char $@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.
    // Original 10 char {'@', '%', '#', '*', '+', '=', '-', ':', '.', ' ',' ' };
    
    // high-key and normal images
    char regularMap[] {'W', '#', '@', '0', '&', '%', '*', 'z', '=', '+', '_', ',', '.', ' ' };
    
    // Ideal for a low-key images
    char reverseMap[] {' ', '.', ',', '_', '+', '=', 'z', '*', '%', '&', '0', '@', '#', 'W' };
    
    if(reverse)
        return reverseMap[int(floor(pixel / (255 / (sizeof(reverseMap) - 1))))];
    else
        return regularMap[int(floor(pixel / (255 / (sizeof(regularMap) - 1))))];
}



// Drawing in a file - Main output
void WriteToFile(std::vector<char> image, int imageWidth, const char* filename){
    char str[80];
    strcpy(str, "images/");
    strcat(str, filename);
    strcat(str, ".txt");
    
    std::ofstream myfile (str);
    
    if(myfile.is_open())
    {
        for(int i=1;i<image.size(); i++){
            myfile << image.at(i);
            if (i % (imageWidth) == 0)
                myfile << "\n";
        }
    }else
        std::cout << "Write to file faled\n";
}


