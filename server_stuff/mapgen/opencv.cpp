#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cassert>
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>
#include "hough.h"

#define RHO 0
#define THETA 1

#define M_PI 3.14159265358979323846

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

    std::vector< std::pair<int, int> > coords; // List of x, y coords
    for (int i=0; i<cdst.rows; ++i) {
        for (int j=0; j<cdst.cols; ++j) {
            if (cdst.at<cv::Vec3b>(i,j)[0] == 255) {
                coords.push_back(std::make_pair(j, cdst.rows - i));
            }
        }
    }
    // Now to create a hough table of rho and theta, we need to know
    // the maxVals of both.
    // Our table will be of dimensions max(rho) x max(theta)
    std::vector<float> maxVal(2), res(2);
    maxVal[RHO] = (float) (sqrt(pow(cdst.rows, 2) + pow(cdst.cols, 2)));
    maxVal[THETA] = (float) 2 * M_PI;

    //PPRINT(maxVal[RHO]); 
    //PPRINT(maxVal[THETA]); 
    res[RHO] = 1;
    res[THETA] = M_PI / 180; 

    houghSpace hs (res, maxVal);

    std::vector<float> vote(2);
    float m; // slope
    for (auto &p: coords) {
        /* The theta here is the slope of the line with +X axis
         * We'll convert the line from slope-point form to hesse-normal
         * form for the hough table
         */
        for (float theta = 0; theta < M_PI; theta += M_PI / 180) {
            m = tanf(theta);
            vote[RHO] = std::abs(p.second - m * p.first) / (sqrt(1 + m*m));
            /* Case I: y-intercept is > 0 */
            if ((p.second - m * p.first) > 0) {
                if (theta < M_PI / 2.0) {
                    vote[THETA] = theta + M_PI / 2.0;
                } else {
                    vote[THETA] = theta - M_PI / 2.0;
                }
            }
            /* Case II: y-intercept is < 0 */
            else {
                if (theta < M_PI / 2.0) {
                    vote[THETA] = theta + M_PI + M_PI / 2.0;
                } else {
                    vote[THETA] = theta + M_PI - M_PI / 2.0;
                }
            }
            assert(vote[RHO] <= maxVal[RHO]);
            assert(vote[THETA] <= maxVal[THETA]);
            hs.addVote(vote);
        }
    }
    std::vector< std::vector<float> > vec (hs.getMaxima(atoi(argv[2])));
    for (auto &p: vec) {
        std::cout << p[0] << " "
                  << p[1] << std::endl;
    }

    return 0;
}
