#include "CSVRow.h"
#include "houghalt.h"
#include "payload.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <functional>

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

std::vector< std::pair< std::pair<float, float>, std::pair<float, float> > > getLineSegments(
        std::vector< std::pair< std::vector<float>, std::vector< std::vector<float> > > > maxima)
{
    std::vector< std::pair< std::pair<float, float>, std::pair<float, float> > > ans;
    for (auto &line : maxima) {
        auto xy = std::minmax_element(line.second.begin(), line.second.end());
        ans.push_back(std::make_pair(std::make_pair((*xy.first)[X], (*xy.first)[Y]), std::make_pair((*xy.second)[X], (*xy.second)[Y])));
    }
    return ans;
}

void doHough(std::vector<payload> readings, int lineThresh, int pointThresh)
{
    std::vector<float> maxVal(2), res(2);
    std::vector<float> minVal(2);
    std::vector<float> vote(2);
    std::vector<float> voteLoc(2);

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
            voteLoc[X] = vote[RHO] * cosf(vote[THETA]);
            voteLoc[Y] = vote[RHO] * sinf(vote[THETA]);
            assert(vote[RHO] <= maxVal[RHO]);
            assert(vote[THETA] <= maxVal[THETA]);
            PPRINT(vote[RHO]);
            PPRINT(vote[THETA]);
            PPRINT(voteLoc[X]);
            PPRINT(voteLoc[Y]);
            linespace.addVote(vote, voteLoc);
        }
    }
    auto lineF (linespace.getMaxima(lineThresh));
    std::ofstream lout ("lines.txt");
    for (auto &p: lineF) {
        for (auto &q: p.first) {
            lout << q << " ";
        }
        lout << std::endl;
    }
    std::ofstream lsout("line_segments.txt");
    auto lineSegments = getLineSegments(lineF);
    for (auto &ls : lineSegments) {
        lsout   << ls.first.first   << ' ' //X1
                << ls.first.second  << ' ' //Y1
                << ls.second.first  << ' ' //X2
                << ls.second.second << ' ' //Y2
                << std::endl;
    }
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
