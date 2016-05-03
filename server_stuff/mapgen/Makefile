IDIR=include


CXX=g++
CXXFLAGS=-std=c++11 -O3 -I. -I$(IDIR)

BUILDDIR=build

.PHONY: clean

all: $(BUILDDIR)/houghmain $(BUILDDIR)/opencv $(BUILDDIR)/ekftest $(BUILDDIR)/ekfmain | $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/ekf.o: $(IDIR)/ekf.cpp $(IDIR)/ekf.hpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $(IDIR)/ekf.cpp -o $(BUILDDIR)/ekf.o

$(BUILDDIR)/ekftest.o: ekf-test.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c ekf-test.cpp -o $(BUILDDIR)/ekftest.o

$(BUILDDIR)/ekftest: $(BUILDDIR)/ekf.o $(BUILDDIR)/ekftest.o | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(BUILDDIR)/ekf.o $(BUILDDIR)/ekftest.o -o $(BUILDDIR)/ekftest

$(BUILDDIR)/ekfmain.o: ekf-main.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c ekf-main.cpp -o $(BUILDDIR)/ekfmain.o

$(BUILDDIR)/ekfmain: $(BUILDDIR)/ekf.o $(BUILDDIR)/CSVRow.o $(BUILDDIR)/ekfmain.o | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(BUILDDIR)/ekf.o $(BUILDDIR)/CSVRow.o $(BUILDDIR)/ekfmain.o -o $(BUILDDIR)/ekfmain

$(BUILDDIR)/houghmain: $(BUILDDIR)/hough.o $(BUILDDIR)/CSVRow.o $(BUILDDIR)/houghmain.o | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(BUILDDIR)/CSVRow.o $(BUILDDIR)/hough.o $(BUILDDIR)/houghmain.o -o $(BUILDDIR)/houghmain

$(BUILDDIR)/houghmain.o: houghmain.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c houghmain.cpp -o $(BUILDDIR)/houghmain.o

$(BUILDDIR)/opencv: $(BUILDDIR)/hough.o $(BUILDDIR)/opencv.o | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags --libs opencv` $(BUILDDIR)/hough.o $(BUILDDIR)/opencv.o -o $(BUILDDIR)/opencv

$(BUILDDIR)/opencv.o: opencv.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) `pkg-config --cflags --libs opencv` -c opencv.cpp -o $(BUILDDIR)/opencv.o

$(BUILDDIR)/hough.o: $(IDIR)/hough.h $(IDIR)/hough.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $(IDIR)/hough.cpp -o $(BUILDDIR)/hough.o

$(BUILDDIR)/CSVRow.o: $(IDIR)/CSVRow.cpp $(IDIR)/CSVRow.h | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $(IDIR)/CSVRow.cpp -o $(BUILDDIR)/CSVRow.o

clean:
	rm -rf $(BUILDDIR)
