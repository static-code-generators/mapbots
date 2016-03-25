CC=gcc
CCLAGS=-std=c99

all: dir server

dir:
	mkdir -p build

server: dir server.c payload.h
	$(CC) $(CCFLAGS) server.c -o build/server

clean:
	rm -rf build
