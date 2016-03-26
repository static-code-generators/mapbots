IDIR=include

CPP=g++
CPPFLAGS=-std=c++11 -O3 -I. -I$(IDIR)

BUILDDIR=build

all: dir $(BUILDDIR)/hough.o $(BUILDDIR)/CSVRow.o

dir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/houghmain: dir $(BUILDDIR)/hough.o $(BUILDDIR)/CSVRow.o $(BUILDDIR)/houghmain.o
	$(CPP) $(CPPFLAGS) $(BUILDDIR)/CSVRow.o $(BUILDDIR)/hough.o $(BUILDDIR)/houghmain.o -o $(BUILDDIR)/houghmain

$(BUILDDIR)/houghmain.o: dir houghmain.cpp
	$(CPP) $(CPPFLAGS) -c houghmain.cpp -o $(BUILDDIR)/houghmain.o

$(BUILDDIR)/opencv: dir $(BUILDDIR)/hough.o $(BUILDDIR)/opencv.o
	$(CPP) $(CPPFLAGS) `pkg-config --cflags --libs opencv` $(BUILDDIR)/hough.o $(BUILDDIR)/opencv.o -o $(BUILDDIR)/opencv

$(BUILDDIR)/opencv.o: dir opencv.cpp
	$(CPP) $(CPPFLAGS) `pkg-config --cflags --libs opencv` -c opencv.cpp -o $(BUILDDIR)/opencv.o

$(BUILDDIR)/hough.o: dir $(IDIR)/hough.h $(IDIR)/hough.cpp
	$(CPP) $(CPPFLAGS) -c $(IDIR)/hough.cpp -o $(BUILDDIR)/hough.o

$(BUILDDIR)/CSVRow.o: dir $(IDIR)/CSVRow.cpp $(IDIR)/CSVRow.h
	$(CPP) $(CPPFLAGS) -c $(IDIR)/CSVRow.cpp -o $(BUILDDIR)/CSVRow.o

clean:
	rm -rf build
