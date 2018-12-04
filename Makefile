OPENCV_LIB := $(shell pkg-config --cflags --libs opencv)
CC=g++ -std=c++17 -O2 -fopenmp
#CC=clang++ -Xpreprocessor -fopenmp
#CC=/usr/local/bin/g++-8 -std=c++11 -fopenmp

main: main.cpp utils.cpp live.cpp image.cpp video.cpp 
	$(CC) main.cpp utils.cpp live.cpp image.cpp video.cpp $(OPENCV_LIB) -o pixels

debug: main.cpp utils.cpp live.cpp image.cpp video.cpp
	$(CC) main.cpp utils.cpp live.cpp image.cpp video.cpp $(OPENCV_LIB) -o pixels -g

