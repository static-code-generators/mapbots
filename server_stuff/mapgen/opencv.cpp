#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cassert>
#include <vector>
#include <utility>
#include "hough.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "USAGE: " << argv[0] << " <filename>"
                  << std::endl;
        return -1;
    }
    const char *filename = argv[1];
    cv::Mat src = cv::imread(filename, 0);
    assert(!src.empty());
    cv::Mat dst, cdst;
    cv::Canny(src, dst, 50, 200, 3);
    cv::cvtColor(dst, cdst, CV_GRAY2BGR);

    cv::imwrite("cdst.jpg", cdst);
    
    std::vector< std::pair<int, int> > coords; // List of x, y coords
    for (int i=0; i<cdst.rows; ++i) {
        uchar *p = cdst.ptr(i);
        for (int j=0; j<cdst.cols; ++j) {
            if (p[j] == 255) {
                coords.push_back(std::make_pair(j, cdst.rows - i));
            }
        }
    }
    for (auto &p: coords) {
        std::cout << p.first << ", " << p.second << std::endl;
    }
    //std::cout << cdst;
    return 0;
}
