#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cassert>
#include <vector>
#include <utility>
#include <cmath>
#include "hough.h"

#define RHO 0
#define THETA 1

#define PPRINT(x) std::cout << #x << ": " << x << std::endl

int main(int argc, char **argv)
{
    if (argc < 3) {
        std::cout << "USAGE: " << argv[0] << " <filename> <threshold>"
                  << std::endl;
        return -1;
    }
    const char *filename = argv[1];
    cv::Mat src = cv::imread(filename, 0);
    assert(!src.empty());
    cv::Mat dst, cdst;
    cv::Canny(src, dst, 50, 200, 3);
    cv::cvtColor(dst, cdst, CV_GRAY2BGR);

    //cv::imwrite("cdst.jpg", cdst);
    
    std::vector< std::pair<int, int> > coords; // List of x, y coords
    for (int i=0; i<cdst.rows; ++i) {
        uchar *p = cdst.ptr(i);
        for (int j=0; j<cdst.cols; ++j) {
            if (p[j] == 255) {
                coords.push_back(std::make_pair(j, cdst.rows - i));
            }
        }
    }
    // Now to create a hough table of rho and theta, we need to know
    // the maxVals of both.
    // Our table will be of dimensions max(rho) x max(theta)
    std::vector<float> maxVal(2), res(2);
    maxVal[RHO] = (float) 2 * (sqrt(pow(cdst.rows, 2) + pow(cdst.cols, 2)));
    maxVal[THETA] = (float) 2 * M_PI;

    PPRINT(maxVal[RHO]); 
    PPRINT(maxVal[THETA]); 
    // Let's arbitrarily define the resolution of the table
    res[RHO] = 1;
    res[THETA] = M_PI / 180; 

    houghSpace hs (res, maxVal);

    std::vector<float> vote(2);
    for (auto &p: coords) {
        for (float theta = 0; theta < 2 * M_PI; theta += M_PI / 180) {
            vote[RHO] = abs(p.first * cosf(theta) +  p.second + sinf(theta));
            vote[THETA] = theta;
            assert(vote[RHO] <= maxVal[RHO]);
            assert(vote[THETA] <= maxVal[THETA]);
            hs.addVote(vote);
        }
    }
    std::vector< std::vector<float> > vec (hs.getMaxima(atoi(argv[2])));
    for (auto &p: vec) {
        for (auto &q: p) {
            std::cout << q << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
