IDIR=include

CPP=g++
CPPFLAGS=-std=c++11 -O3 -I. -I$(IDIR)

BUILDDIR=build

all: dir $(BUILDDIR)/hough.o $(BUILDDIR)/CSVRow.o

dir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/hough.o: dir $(IDIR)/hough.h $(IDIR)/hough.cpp
	$(CPP) $(CPPFLAGS) -c $(IDIR)/hough.cpp -o $(BUILDDIR)/hough.o

$(BUILDDIR)/CSVRow.o: dir $(IDIR)/CSVRow.cpp $(IDIR)/CSVRow.h
	$(CPP) $(CPPFLAGS) -c $(IDIR)/CSVRow.cpp -o $(BUILDDIR)/CSVRow.o

clean:
	rm -rf build
