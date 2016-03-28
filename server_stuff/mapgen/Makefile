IDIR=include


CXX=g++
CXXFLAGS=-std=c++11 -O3 -I. -I$(IDIR)
OPENCVFLAGS=`pkg-config --cflags --libs opencv`

BUILDDIR=build

.PHONY: clean

all: $(BUILDDIR)/houghmain $(BUILDDIR)/houghcircle $(BUILDDIR)/opencv | $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/houghmain: $(BUILDDIR)/hough.o $(BUILDDIR)/CSVRow.o $(BUILDDIR)/houghmain.o | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(BUILDDIR)/CSVRow.o $(BUILDDIR)/hough.o $(BUILDDIR)/houghmain.o -o $(BUILDDIR)/houghmain

$(BUILDDIR)/houghmain.o: houghmain.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c houghmain.cpp -o $(BUILDDIR)/houghmain.o

$(BUILDDIR)/houghcircle: $(BUILDDIR)/hough.o $(BUILDDIR)/houghcircle.o | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(BUILDDIR)/hough.o $(BUILDDIR)/houghcircle.o -o $(BUILDDIR)/houghcircle

$(BUILDDIR)/houghcircle.o: houghcircle.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c houghcircle.cpp -o $(BUILDDIR)/houghcircle.o

$(BUILDDIR)/opencv: $(BUILDDIR)/hough.o $(BUILDDIR)/opencv.o | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(OPENCVFLAGS) $(BUILDDIR)/hough.o $(BUILDDIR)/opencv.o -o $(BUILDDIR)/opencv

$(BUILDDIR)/opencv.o: opencv.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c opencv.cpp -o $(BUILDDIR)/opencv.o

$(BUILDDIR)/hough.o: $(IDIR)/hough.h $(IDIR)/hough.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $(IDIR)/hough.cpp -o $(BUILDDIR)/hough.o

$(BUILDDIR)/CSVRow.o: $(IDIR)/CSVRow.cpp $(IDIR)/CSVRow.h | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $(IDIR)/CSVRow.cpp -o $(BUILDDIR)/CSVRow.o

clean:
	rm -rf $(BUILDDIR)
