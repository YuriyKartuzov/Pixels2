// Yuriy Kartuzov GPU621 Project

#include <iostream>
#include <unistd.h>
#include <fstream>
#include "loadpng.h"

int main(int argc, const char * argv[]) {
    // Welcome message
    std::cout << "Pixels 2!\n";
    
    // Filename
    const char* filename = "test1.png";
    printf("file name is: %s\n", filename);
    
    // Declarations
    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;
    
    // Reading Image
    unsigned error = lodepng::decode(image, width, height, filename);
    if(error) {
        std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 0;
    }
    
    // Printing Image details
    printf("The image's width: %d\n", width);
    printf("The image's height: %d\n", height);
    printf("There are %d of RGBA struct\n", (int)(image.size() / 4));
    
    
    // Getting a single Luminocity out of RGBA set. Using standard formula (0.2126*R + 0.7152*G + 0.0722*B)
    std::vector<int> bwImage;
    for(int i=0; i<image.size(); i+=4){
        bwImage.push_back((int)
                          (0.2126 * (int)image.at(i)) + (0.7152 * (int)image.at(i+1)) + (0.0722 * (int)image.at(i+2))
                          );
    }
    
    
    // Mapping to 10 Char ASCII map
    // $@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'.  70 char set
    char map[] {'@', '%', '#', '*', '+', '=', '-', ':', '.', ' ' };
    std::vector<char> output;
    for(int i=0; i<bwImage.size(); i++){
        char character;
        int lum = bwImage.at(i);
        
        if(lum < 25 )
            character = map[0];
        else if(lum >= 25 && lum < 50 )
            character = map[1];
        else if(lum >=50 && lum < 75 )
            character = map[2];
        else if(lum >= 75 && lum < 100 )
            character = map[3];
        else if(lum >= 100 && lum < 125 )
            character = map[4];
        else if(lum >= 125 && lum < 150 )
            character = map[5];
        else if(lum >= 150 && lum < 175 )
            character = map[6];
        else if(lum >= 175 && lum < 200 )
            character = map[7];
        else if(lum >= 200 && lum < 225 )
            character = map[8];
        else if(lum >= 225 )
            character = map[9];
        
        output.push_back(character);
    }
    
    
    // Outputting to a file
    std::ofstream myfile ("output.txt");
    if(myfile.is_open()){
        for(int i=1;i<output.size(); i++){
            myfile << output.at(i);
            if (i % (width) == 0)
                myfile << "\n";
        }
    } else
        std::cout << "DID NOT WORK\n";
    
    return 0;
}
