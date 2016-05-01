CC=gcc
CFLAGS=-std=c99

BUILDDIR=build

all: $(BUILDDIR)/server $(BUILDDIR)/btserver

$(BUILDDIR):
	mkdir -p build

$(BUILDDIR)/server: server.c payload.h | $(BUILDDIR)
	$(CC) $(CFLAGS) server.c -o $(BUILDDIR)/server

$(BUILDDIR)/btserver: bt-server.c payload.h | $(BUILDDIR)
	$(CC) $(CFLAGS) bt-server.c -o $(BUILDDIR)/btserver

clean:
	rm -rf build
