OPENCV_LIB := $(shell pkg-config --cflags --libs opencv)
CC=g++ -std=c++17
main: main.cpp utils.cpp live.cpp image.cpp
	$(CC) main.cpp utils.cpp live.cpp image.cpp $(OPENCV_LIB) -o pixels

debug: main.cpp utils.cpp live.cpp image.cpp
	$(CC) main.cpp utils.cpp live.cpp image.cpp $(OPENCV_LIB) -g -o pixels