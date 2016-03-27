CC=gcc
CFLAGS=-std=c99

BUILDDIR=build

all: $(BUILDDIR)/server

$(BUILDDIR):
	mkdir -p build

$(BUILDDIR)/server: server.c payload.h | $(BUILDDIR)
	$(CC) $(CFLAGS) server.c -o $(BUILDDIR)/server

clean:
	rm -rf build
