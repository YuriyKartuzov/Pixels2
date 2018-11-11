#include "utils.h"

using namespace cv;

vector<char> reduceAndMap(
        vector<int>imageV, int  width, int height, int & newW, int & newH, int & asciiW, int & asciiH, int & xr, int & yr, char * size){

    std::vector<char> mappedImage;
    int xP = 0; // Main index for treversing the image
    int sum = 0;

    int xratio, yratio;
    int horozinalFactor, verticalFactor;

    if( !strcmp(size, "small")) { 
        horozinalFactor = 120; 
        verticalFactor = 100;
    }
    else if(! strcmp(size, "medium")){
        horozinalFactor = 190; 
        verticalFactor = 140;
    }
    else { // Large
        horozinalFactor = 270;
        verticalFactor = 180;
    }

    // Calculate dimentions for chunk to average
    if( (width / height) < 0){
        yratio = int(floor(width / horozinalFactor));
        xratio = int(yratio / 2);
    } else {
        xratio = int(floor(height / verticalFactor));
        yratio = int(xratio * 2);
    }
    
    // Calculate new width and height
    int newWidth = (xratio * (int)(floor(width / xratio)));
    int newHeight = (yratio * (int)(floor(height / yratio)));

    // Crop immage to be multiple of xratio and yratio
    vector<int> croppedImage;
    vector<int>::const_iterator current = imageV.begin();
    for(int i = 0 ; i < height; i++){
        copy(current, current + newWidth, std::back_inserter(croppedImage));
        current += width;
    }

    // Writing value back to calling funciton
    newW = newWidth;
    newH = newHeight;
    asciiW = (int)(newWidth / xratio);
    asciiH = (int)(newHeight / yratio);
    xr = xratio;
    yr = yratio;

    
    // Row block treversal
    for(int y = 0; xP < croppedImage.size() - (yratio * newW); y++){
        
        // Colomn block treversal
        for(int x = 0; x < (newW / xratio); x++){
            
            // Rows treversal
            for(int i = 0; i < yratio; i++){
                
                // Colomn treversal
                for(int j = 0; j < xratio; j++){
                    sum += croppedImage.at(xP + j);
                }
                
                xP += newW;
            }
            
            // Getting average and mapping to CHAR
            int average = (int)sum / (xratio * yratio);
            mappedImage.push_back( pixelToChar(average, false) );
            
            // reposition & reset
            xP = xP - (yratio * newW) + xratio; // ход конем
            sum = 0;
        }
        
        xP += (yratio * newW);
    }
    return mappedImage;
}



// Mapping to ASCII --------------------------------------------------------------
char pixelToChar(int pixel, bool reverse) {
   
    char regularMap[] = {'W', '#', '@', '0', '&', '%', '*', 'z', '=', '+', '_', ',', '.', ' ' };
    char reverseMap[] = {' ', '.', ',', '_', '+', '=', 'z', '*', '%', '&', '0', '@', '#', 'W' };
    
    if(reverse)
        return reverseMap[int(pixel / (255 / (sizeof(reverseMap) - 1)))];
    else
        return regularMap[int(pixel / (255 / (sizeof(regularMap) - 1)))];
}


// Pixelsfrom ascii char    --------------------------------------------------------------
vector<unsigned char> asciiToPixels(vector<char>a, int & asciiWidth, int & asciiHeight, int &finalHeight, int & finalWidth){

    // Main output variable
    vector<unsigned char> pixels;

    int letterWidth = 13;
    int letterHeight = 18;

    // Letter initialization
    vector< vector<unsigned char> > letterMap = getPixelsForLetter();

    
    // Go through each line of ASCII chars
    for(int c=0; c < asciiHeight; c++){

        vector<unsigned char> line (a.begin() + (c * asciiWidth), a.begin() + (c * asciiWidth) + asciiWidth);

        for(int x=0; x< letterHeight; x++){

            // Iterating over a line of letters
            for(int i=0; i<line.size(); i++){

                for(int j=0; j<letterWidth; j++){
                    if(line.at(i) == 'W') pixels.push_back(letterMap.at(0).at(x*letterWidth + j));
                    if(line.at(i) == '#') pixels.push_back(letterMap.at(1).at(x*letterWidth + j));
                    if(line.at(i) == '@') pixels.push_back(letterMap.at(2).at(x*letterWidth + j));
                    if(line.at(i) == '0') pixels.push_back(letterMap.at(3).at(x*letterWidth + j));
                    if(line.at(i) == '&') pixels.push_back(letterMap.at(4).at(x*letterWidth + j));
                    if(line.at(i) == '%') pixels.push_back(letterMap.at(5).at(x*letterWidth + j));
                    if(line.at(i) == '*') pixels.push_back(letterMap.at(6).at(x*letterWidth + j));
                    if(line.at(i) == 'z') pixels.push_back(letterMap.at(7).at(x*letterWidth + j));
                    if(line.at(i) == '=') pixels.push_back(letterMap.at(8).at(x*letterWidth + j));
                    if(line.at(i) == '+') pixels.push_back(letterMap.at(9).at(x*letterWidth + j));
                    if(line.at(i) == '_') pixels.push_back(letterMap.at(10).at(x*letterWidth + j));
                    if(line.at(i) == ',') pixels.push_back(letterMap.at(11).at(x*letterWidth + j));
                    if(line.at(i) == '.') pixels.push_back(letterMap.at(12).at(x*letterWidth + j));
                    if(line.at(i) == ' ') pixels.push_back(letterMap.at(13).at(x*letterWidth + j));
                }
            }
        }
    }

    finalWidth =  asciiWidth * letterWidth;
    finalHeight = pixels.size() / finalWidth;

    return pixels;
}

// Reading files for a letter  --------------------------------------------------------------
vector<vector<unsigned char> > getPixelsForLetter(){
    string base = "chars/";
    string files[14] = {"W", "Pound", "At", "Zero", "Amp", "Perc", "Star", "Zed", "Equal", "Plus", "Under", "Comma", "Period", "Space"};
    string extension = ".png";
    
    vector<vector<unsigned char> > map;

    try{
        for(int i=0; i<14; i++){
            Mat mat = imread(base + files[i] + extension, CV_LOAD_IMAGE_GRAYSCALE);
            if(!mat.data){
                cerr << "ERROR: file [" << base + files[i] + extension << "] could not be read\n";
                throw;
            }

            vector<unsigned char>letter(mat.begin<unsigned char>(), mat.end<unsigned char>());
            map.push_back(letter);
        }
    }catch(exception e){
        cerr << "ERROR: Missing font files.\n" << e.what() << endl;
    }

    return map;
}

