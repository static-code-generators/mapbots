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
        assert(row.size() == 5);
        p = {.bot_id = stoi(row[0]),
             .reading = stof(row[1]),
             .loc = {.x = stof(row[2]),
                     .y = stof(row[3]),
                     .theta = stof(row[4])}};
        readings.push_back(p);
    }
    return readings;
}

void addVotes(std::vector<payload> readings)
{
    std::vector<float> maxVal(2), res(2);
    std::vector<float> vote(2);

    /**
     * LINE SPACE
     * RHO: The perpendicular distance of the line from origin
     * THETA: Angle the perpendicular from the origin to the line
     *        makes with +X axis
     *
     * beta: Angular error of ultrasonic sensor, -15deg < beta < 15deg
     */
    maxVal[RHO] = 1500.0F; // in centimetres
    maxVal[THETA] = (float) M_PI; // in radians
    res[RHO] = 1;
    res[THETA] = M_PI / 180;

    houghSpace linespace (res, maxVal);

    for (auto &p: readings) {
        for (float beta = -M_PI/12; beta <= M_PI/12; beta += M_PI/180) {
            // Refer to the paper for the derivation
            vote[THETA] = p.loc.theta + beta;
            if (vote[THETA] < 0) {
                vote[THETA] += M_PI;
            } else if (vote[THETA] > M_PI) {
                vote[THETA] -= M_PI;
            }
            vote[RHO] = p.reading + (p.loc.x * cosf(vote[THETA]))
                                  + (p.loc.y * sinf(vote[THETA]));
            assert(vote[RHO] <= maxVal[RHO]);
            assert(vote[THETA] <= maxVal[THETA]);
            PPRINT(vote[RHO]);
            PPRINT(vote[THETA]);
            linespace.addVote(vote);
        }
    }
    //std::cout << linespace;
    /**
     * POINT SPACE
     * X: X-coordinate of point
     * Y: Y-coordinate of point
     *
     * beta: Angular error of ultrasonic sensor, -15deg < beta < 15deg
     */
    maxVal[X] = 1500.0F; // in centimetres
    maxVal[Y] = 1500.0F; // in centimetres
    res[X] = 1;
    res[Y] = 1;

    houghSpace pointspace (res, maxVal);

    for (auto &p: readings) {
        for (float beta = -M_PI/12; beta <= M_PI/12; beta += M_PI/180) {
            vote[X] = p.loc.x + (p.reading * cosf(p.loc.theta + beta));
            vote[Y] = p.loc.y + (p.reading * sinf(p.loc.theta + beta));
            assert(vote[X] <= maxVal[X]);
            assert(vote[Y] <= maxVal[Y]);
            PPRINT(vote[X]);
            PPRINT(vote[Y]);
            pointspace.addVote(vote);
        }
    }
    //std::cout << pointspace;
}

int main(int argc, char **argv)
{
    assert(argc >= 2);
    std::vector<payload> readings = readCSV(argv[1]);
    for (auto &p: readings) {
        std::cout << p.bot_id << " "
                  << p.reading << " "
                  << p.loc.x << " "
                  << p.loc.y << " "
                  << p.loc.theta << " " << std::endl;
    }
    addVotes(readings);
    return 0;
}
