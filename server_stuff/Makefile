CC=gcc
CCLAGS=-std=c99

all: server

server: server.c payload.h
	mkdir -p build
	$(CC) $(CCFLAGS) server.c -o build/server

clean:
	rm -rf build
