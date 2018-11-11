#include <iostream>
using namespace std;

// Prototypes
void liveVideoProcessing(int deviceNum, char * size);
void imageProcessing(const char *filename, char * size);

//MAIN
int main(int argc, const char * argv[])
{

    if(argc < 2){
        cout << "\n\n Welcome to PIXELS2, it can do one of the following:\n";
        cout << "1. Image into ASCII art. Launch command: `px image r2d2 [small,medium,large]`. PNG extention is assumed. [] choose one optional value. Large is defualt\n";
        cout << "2. Process video feed. Launch statement `px live [0-10]`. Optional int represents device# aka camera ID. 0 is default\n";
        cout << "3. Turns video into ascii video (TO BE COMPLETED)\n\n";
        return -1;
    }

    // Live processing - live.cpp
    if( !strcmp(argv[1], "live")){
        char size[10];
        strcpy(size, "small");

        int deviceNum = 0;

        if(argc == 3)
            deviceNum = atoi(argv[2]);
\
        liveVideoProcessing(deviceNum, size); 
    }

    // Image processing - image.cpp
    if( !strcmp(argv[1], "image")){
        char size[10];
        
        if(argc == 4) 
            strcpy(size, argv[3]);
        else 
            strcpy(size, "large");

        imageProcessing(argv[2], size); // passing in filename and size
    }

    // Video processing - video.cpp
    if( !strcmp(argv[1], "video")){
        // add function //passing in filename
    }

    return 0;
}
