#include <iostream>
#include <omp.h>
#include <cstring>

using namespace std;

// Prototypes
void live(int argc, const char * argv[]);
void video(int argc, const char * argv[]);
void imageProcessing(const char * filename);

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
        live(argc, argv);
    }


    // Video processing - video.cpp
    if( !strcmp(argv[1], "video")){
        if(argc < 3){
            cerr << "Missing video filename. MP4 extention is assumed";
            return -1;
        }
        video(argc, argv);
    }


    // Image processing - image.cpp
    if( !strcmp(argv[1], "image")){
        char size[10];
        string size1 = "large";
        
        if(argc == 4) 
            //strcpy_s(size, argv[3]);
            size1 = argv[3];
        //else 
            //strcpy_s(size, "large");

        imageProcessing(argv[2]); // passing in filename and size
    }

    return 0;
}
