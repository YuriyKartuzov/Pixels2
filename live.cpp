#include "utils.h"
#include <omp.h>

using namespace std;
using namespace cv;
using namespace chrono;


void live(int argc, const char * argv[]) {
	// Setting up camera
	int deviceNum = 0;
	int frameWidth = 640;
	int frameHeight = 480;

	if (argc == 3)
		deviceNum = atoi(argv[2]);

	if (argc == 4 && !strcmp(argv[3], "large")) {
		frameWidth = 1920;
		frameHeight = 1080;
		deviceNum = atoi(argv[2]);
	}

	// Video feed, Access camera and validation
	VideoCapture cam(deviceNum);
	if (!cam.isOpened()) {
		cout << "Error opening video stream\n" << endl;
		return;
	}

	// Set frame dimentions
	cam.set(CAP_PROP_FRAME_WIDTH, frameWidth);
	cam.set(CAP_PROP_FRAME_HEIGHT, frameHeight);

	// Check frame dimentions set by openCV
	int originalWidth = cam.get(CAP_PROP_FRAME_WIDTH);
	int originalHeight = cam.get(CAP_PROP_FRAME_HEIGHT);
	int width, height;

	// Reading char files and creating matrix of ascii pixels, and updating fontWidth and fontHeight vars below
	int fontWidth;
	int fontHeight;
	Mat *characters = new Mat[14];
	int numOfChars = loadCharacters(characters, fontWidth, fontHeight);

	// Calculating new width and height for frame resizing
	if (originalWidth % fontWidth == 0)
		width = originalWidth;
	else
		width = int(originalWidth / fontWidth) * fontWidth;

	if (originalHeight % fontHeight == 0)
		height = originalHeight;
	else
		height = int(originalHeight / fontHeight) * fontHeight;

	// Output frame
	unsigned char * output = new unsigned char[width * height];

	// REPORT
	printf("Pixels 2.0 program: LIVE!\nFrame width: %d, height: %d\n", originalWidth, originalHeight);

	// Video writer
	// remove("liveout.mp4");
	// cv::namedWindow("Live shit", cv::WINDOW_AUTOSIZE);
	// VideoWriter videoOut("liveout.mp4", CV_FOURCC('m','j','p','g'), 25, Size(width, height), 0); 


	// Declarations used in loop
	int c = 0;
	int cc = 0;
	Mat frame, bw, cropped;
	double cropT = 0, greyT = 0, proccT = 0;
	int sampleRate = 30;

	//#pragma omp single
	while (1) {
		c++;
		cc++;

		// FRAME Input
		bool isSuccess = cam.read(frame);
		if (!isSuccess)
			break;

		// Turn into black and white
		auto start = high_resolution_clock::now();
		cv::cvtColor(frame, bw, COLOR_BGR2GRAY);
		if (c % sampleRate == 0) {
			long long micro = duration_cast<microseconds>(high_resolution_clock::now() - start).count();
			printf("Frame #: %d, OpenCV toGrey(): %lld,", cc, micro);
			greyT += micro;
		}

		// Crop the image
		start = high_resolution_clock::now();
		//cv::resize(bw, cropped, cv::Size(width, height), 0, 0, INTER_CUBIC); COMPARED 500 vs 200 microsecs for mine
		unsigned char * croppedImage = cropImage(bw.data, originalWidth, originalHeight, width, height);
		if (c % sampleRate == 0) {
			long long micro = duration_cast<microseconds>(high_resolution_clock::now() - start).count();
			printf(" crop(): %lld,", micro);
			cropT += micro;
		}

		start = high_resolution_clock::now();
		//Processing - output is updated. TIME THIS FUNCTION
		imageProcessing(croppedImage, output, characters, width, height, fontWidth, fontHeight, numOfChars);
		if (c % sampleRate == 0) {
			long long micro = duration_cast<microseconds>(high_resolution_clock::now() - start).count();
			printf(" toASCII(): %lld micro-seconds.\n", micro);
			c = 0;
			proccT += micro;
		}

		// Create new frame with output
		cv::Mat processedFrame(height, width, CV_8UC1, output);

		// Show frame
		cv::imshow("Live Stream Processing", processedFrame);
		//videoOut.write(processedFrame);

		// Escape
		if (waitKey(10) == 27) {
			printf("ESC pressed. Bye\n\n");
			break;
		}
		//video.release();
	}
}
