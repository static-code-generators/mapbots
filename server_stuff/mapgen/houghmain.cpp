#include "CSVRow.h"
#include "hough.h"
#include "payload.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

#define RHO 0
#define THETA 1

#define X 0
#define Y 1

#define M_PI 3.14159265358979323846

#define PPRINT(x) std::cout << #x << ": " << x << std::endl

std::vector<payload> readCSV(char *filename)
{
    std::ifstream fin (filename);
    CSVRow row;
    payload p;
    std::vector<payload> readings;
    while (fin >> row)
    {
        // No bot_id anymore
        assert(row.size() >= 4);
        p = {.reading = stof(row[0]),
             .loc = {.x = stof(row[1]),
                     .y = stof(row[2]),
                     .theta = stof(row[3])}};
        if (p.reading > 0)
            readings.push_back(p);
    }
    return readings;
}

void doHough(std::vector<payload> readings, int lineThresh, int pointThresh)
{
    std::vector<float> maxVal(2), res(2);
    std::vector<float> minVal(2);
    std::vector<float> vote(2);

    /**
     * LINE SPACE
     * RHO: The perpendicular distance of the line from origin
     * THETA: Angle the perpendicular from the origin to the line
     *        makes with +X axis
     *
     * beta: Angular error of ultrasonic sensor, -15deg < beta < 15deg
     */
    maxVal[RHO] = 15000.0F; // in millimetres
    maxVal[THETA] = (float) 2 * M_PI; // in radians
    res[RHO] = 100;
    res[THETA] = M_PI / 180 * 5;

    houghSpace linespace (res, maxVal);

    for (auto &p: readings) {
        for (float beta = -M_PI/12; beta <= M_PI/12; beta += M_PI/180) {
            // Refer to the paper for the derivation
            vote[THETA] = p.loc.theta + beta;
            vote[THETA] = fmod(vote[THETA], 2 * M_PI);
            if (vote[THETA] < 0) {
                vote[THETA] += 2 * M_PI;
            }
            vote[RHO] = std::abs(p.reading + (p.loc.x * cosf(vote[THETA]))
                                  + (p.loc.y * sinf(vote[THETA])));
            assert(vote[RHO] <= maxVal[RHO]);
            assert(vote[THETA] <= maxVal[THETA]);
            PPRINT(vote[RHO]);
            PPRINT(vote[THETA]);
            linespace.addVote(vote);
        }
    }
    std::vector< std::vector<float> > lineF (linespace.getMaxima(lineThresh));
    std::ofstream lout ("lines.txt");
    for (auto &p: lineF) {
        for (auto &q: p) {
            lout << q << " ";
        }
        lout << std::endl;
    }
    //std::cout << linespace;
    /**
     * POINT SPACE
     * X: X-coordinate of point
     * Y: Y-coordinate of point
     *
     * beta: Angular error of ultrasonic sensor, -15deg < beta < 15deg
     */
//    maxVal[X] = 15000.0F; // in millimetres
//    maxVal[Y] = 15000.0F; // in millimetres
//    minVal[X] = -15000.0F;
//    minVal[Y] = -15000.0F;
//    res[X] = 10;
//    res[Y] = 10;
//
//    houghSpace pointspace (res, maxVal, minVal);
//
//    for (auto &p: readings) {
//        for (float beta = -M_PI/12; beta <= M_PI/12; beta += M_PI/180) {
//            vote[X] = p.loc.x + (p.reading * cosf(p.loc.theta + beta));
//            vote[Y] = p.loc.y + (p.reading * sinf(p.loc.theta + beta));
//            assert(vote[X] <= maxVal[X]);
//            assert(vote[Y] <= maxVal[Y]);
//            PPRINT(vote[X]);
//            PPRINT(vote[Y]);
//            pointspace.addVote(vote);
//        }
//    }
//    std::vector< std::vector<float> > pointF (pointspace.getMaxima(pointThresh));
//    std::ofstream pout ("points.txt");
//    for (auto &p: pointF) {
//        for (auto &q: p) {
//            pout << q << " ";
//        }
//        pout << std::endl;
//    }
  //std::cout << pointspace;
}

/* argv: binname file.csv l-threshold p-threshold */
int main(int argc, char **argv)
{
    assert(argc == 4);
    std::vector<payload> readings = readCSV(argv[1]);
    int lineThresh = atoi(argv[2]);
    int pointThresh = atoi(argv[3]);
    doHough(readings, lineThresh, pointThresh);
    return 0;
}
