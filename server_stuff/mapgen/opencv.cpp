#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cassert>
#include "hough.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cout << "USAGE: " << argv[0] << " <filename>"
                  << std::endl;
    }
    const char *filename = argv[1];
    cv::Mat src = cv::imread(filename, 0);
    if (src.empty()) {
        assert(0);
    }
    cv::Mat dst, cdst;
    cv::Canny(src, dst, 50, 200, 3);
    cv::cvtColor(dst, cdst, CV_GRAY2BGR);

    cv::imwrite("cdst.jpg", cdst);
    
    for (int i=0; i<cdst.rows; ++i) {
        uchar *p = cdst.ptr(i);
        for (int j=0; j<cdst.cols; ++j) {
            if (p[j] == 255) {
                std::cout << "(" << i << ", " << j
                     << ")" << std::endl;
            }
        }
    }
    //std::cout << cdst;
    return 0;
}
