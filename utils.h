#pragma once
#include "opencv2/opencv.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>
#include "string.h"

using namespace std;

// Main processing function
void imageProcessing(const unsigned char * image, unsigned char * output, const cv::Mat characters[], int width, int height, int fontX, int fontY, int numOfChars);

// Crop image
unsigned char * cropImage(unsigned char * data, int originalW, int originalH, int width, int height);

// Map individual pixels {'W', '#', '@', '0', '&', '%', '*', 'z', '=', '+', '_', ',', '.', ' ' }
char pixelToChar(int pixel, bool reverse);

// Reduce a rectangle of pixels to a single averaged value
vector<char> reduceAndMap(vector<int>imageV, int  w, int h, int & nW, int & nH, int & aW, int & aH, int & x, int & y, char * size);

// Map a single character to pixel representation
vector<unsigned char> asciiToPixels(vector<char>a, int & aW, int & aH, int & fW, int & fH);

// Reading files for mapping letters
vector< vector<unsigned char> > getPixelsForLetter();

// Reading files into MAT object
int loadCharacters(cv::Mat *characters, int & fontX, int & fontY);

