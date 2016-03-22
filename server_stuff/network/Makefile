CC=gcc
CCLAGS=-std=c99

all: network

network: client server

client: client.c payload.h
	mkdir -p build
	$(CC) $(CCFLAGS) client.c -o build/client

server: server.c payload.h
	mkdir -p build
	$(CC) $(CCFLAGS) server.c -o build/server

clean:
	rm -rf build
