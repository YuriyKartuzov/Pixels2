#include "utils.h"
using namespace cv;

void imageProcessing(const unsigned char * image, unsigned char * output, const cv::Mat characters[], int width, int height, int chunkWidth, int chunkHeight, int numOfChars ){

    for (int row = width * chunkHeight; row < (width * height); row += width * chunkHeight){
        for (int column = 0; column < width; column += chunkWidth){
            
            int sum = 0;
            
            // Iterate over for a chunk
            for (int x = 0; x < chunkHeight; x++){
                for (int y = 0; y < chunkWidth; y++){

                    int rowIndex = row - width * chunkHeight;
                    int colIndex = column + y + (x * width);
                    sum += image[rowIndex + colIndex];

                }
            }

            // Get average
            int luminocity = sum / (chunkWidth * chunkHeight);

            // Cast luminocity into range and turn into index
            int charIndex = floor(luminocity / (255 / numOfChars));         
            // Fix out of bound index. TODO possible rework of formula above
            if(charIndex >= numOfChars) charIndex = numOfChars - 1 ;
            

            // Write new ascii in pixels
            for (int x = 0; x < chunkHeight; x++){
                for (int y = 0; y < chunkWidth; y++){

                    int rowIndex = row - width * chunkHeight;
                    int colIndex = (column + y + x * width);
                    unsigned char pixel = characters[charIndex].at<unsigned char>(y + x * chunkWidth);
                    output[rowIndex + colIndex] = pixel;

                }
            }
        }
    }
}


// Reading files with char
int loadCharacters(Mat *characters, int & fontX, int & fontY)
{
    string base = "./chars/";
    string files[] = {"At", "W", "Pound", "Zero", "Amp", "Perc", "Star", "Zed", "Equal", "Plus", "Under", "Comma", "Period", "Space"};
    string extension = ".png";

    int numOfFiles = sizeof(files) / sizeof(files[0]);
    for (int i = 0; i < numOfFiles; i++)
    {
        characters[i] = cv::imread(base + files[i] + extension, cv::IMREAD_GRAYSCALE);

        if (!characters[i].data)
            return 0;
        
    }

    fontX = characters[0].cols;
    fontY = characters[0].rows;

    return numOfFiles;
}


vector<char> reduceAndMap(
        vector<int>imageV, int  width, int height, int & newW, int & newH, int & asciiW, int & asciiH, int & xr, int & yr, char * size){

    std::vector<char> mappedImage;
    int xP = 0; // Main index for treversing the image
    int sum = 0;

    int xratio, yratio;
    int horozinalFactor, verticalFactor;

    if( !strcmp(size, "small")) { 
        horozinalFactor = 100; 
        verticalFactor = 80;
    }
    else if(! strcmp(size, "medium")){
        horozinalFactor = 190; 
        verticalFactor = 140;
    }
    else { // Large
        horozinalFactor = 370;
        verticalFactor = 280;
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
    for(int i = 0 ; i < newHeight; i++){
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


// Reading files for a letter  --------------------------------------------------------------
vector<vector<unsigned char> > getPixelsForLetter(){
    string base = "chars/";
    string files[14] = {"W", "Pound", "At", "Zero", "Amp", "Perc", "Star", "Zed", "Equal", "Plus", "Under", "Comma", "Period", "Space"};
    string extension = ".png";
    
    vector<vector<unsigned char> > map;

    try{
        for(int i=0; i<14; i++){
            Mat mat = imread(base + files[i] + extension, cv::IMREAD_GRAYSCALE);
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


