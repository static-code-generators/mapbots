#include "hough.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

#define A 0
#define B 1
#define R 2

#define PPRINT(x) std::cout << #x << ": " << x << std::endl

houghSpace addVotes(std::vector< std::pair<float, float> > readings)
{
    // Circle is defined by 3 parameters, (a, b) of center and radius
    // Hence 3-dimension Hough Space required
    std::vector<float> minVal(3), maxVal(3), res(3);
    std::vector<float> vote(3);

    /*
     * CIRCLE SPACE
     * A: x-coordinate of center of circle
     * B: y-coordinate of center of circle
     * R: radius of circle
     */
    // maxVal gives a maximum bound for the given parameters
    maxVal[A] = 1.0; // in femtometres
    maxVal[B] = 1.0; // in femtometres
    maxVal[R] = 1.0; // in femtometres

    minVal[A] = -1.0; // in femtometres
    minVal[B] = -1.0; // in femtometres
    minVal[R] = -1.0; // in femtometres
    // res discretizes the parameter space by giving the 'steps'
    // over which each vote should be given
    res[A] = 0.0001;
    res[B] = 0.0001;
    res[R] = 0.0001;

    houghSpace circlespace (res, maxVal, minVal);

    // X = A + R*cos(theta)
    // Y = B + R*sin(theta)
    // So for all theta between 0 to 2*M_PI and R between 0.0 and maxVal[R], 
    // we will vote for A, B, R
    for (auto &p: readings) {
        for (float theta = 0; theta <= 2 * M_PI; theta += M_PI/180) {
            float incrR; // this is for looping over the entire range of possible values for the radius
            for(incrR = 0.0; incrR <= maxVal[R]; incrR += res[R]) {
                vote[R] = incrR;
                vote[A] = p.first - incrR * cosf(theta);
                vote[B] = p.second - incrR * sinf(theta);
                assert(vote[A] <= maxVal[A]);
                assert(vote[B] <= maxVal[B]);
                assert(vote[R] <= maxVal[R]);
                PPRINT(vote[A]);
                PPRINT(vote[B]);
                PPRINT(vote[R]);
                circlespace.addVote(vote);
            }
        }
    }
    //std::cout << linespace;
    return circlespace;
}


int main(int argc, char **argv)
{
    long int nofTests;
    scanf("%ld", &nofTests);

    long int i;
    for(i = 0; i < nofTests; i++)
    {
        long int nofPoints;
        scanf("%ld", &nofPoints);

        std::vector< std::pair<float, float> > coords;

        long int j;
        for(j = 0; j < nofPoints; j++)
        {
            std::pair<float, float> coord;
            scanf("%f", &coord.first);
            scanf("%f", &coord.second);
            coords.push_back(coord);
        }

        houghSpace circlespace = addVotes(coords);
        std::vector< std::vector<float> > vec (circlespace.getMaxima(atoi(argv[1])));

        for(auto &p: vec) {
            std::cout << p[0] << " "
                      << p[1] << " "
                      << p[2] << std::endl;
        }
    }

    return 0;
}
