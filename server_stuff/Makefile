CC=gcc
CCLAGS=-std=c99
CPP=g++
CPPFLAGS=

all: server

server: server.c payload.h
	mkdir -p build
	$(CC) $(CCFLAGS) server.c -o build/server

hough: hough.cpp payload.h
	mkdir -p build
	$(CPP) $(CPPFLAGS) hough.cpp -o build/hough

clean:
	rm -rf build
