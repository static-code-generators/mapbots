CC=gcc
CCLAGS=-std=c99
CPP=g++
CPPFLAGS=-std=c++11 -O3

all: server

server: server.c payload.h
	mkdir -p build
	$(CC) $(CCFLAGS) server.c -o build/server

hough: hough.h hough.cpp
	mkdir -p build
	$(CPP) $(CPPFLAGS) -c hough.cpp
	mv hough.o build

csvrow: CSVRow.cpp CSVRow.h
	mkdir -p build
	$(CPP) $(CPPFLAGS) -c CSVRow.cpp
	mv CSVRow.o build

clean:
	rm -rf build
