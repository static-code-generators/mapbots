#include "CSVRow.h"
#include "hough.h"
#include "payload.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

#define RHO 0
#define THETA 1

std::vector<payload> readCSV(char *filename)
{
    std::ifstream fin (filename);
    CSVRow row;
    payload p;
    std::vector<payload> readings;
    while (fin)
    {
        fin >> row;
        if (!fin) {
            break;
        }
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
    maxVal[RHO] = (float) 1500; // in centimetres
    maxVal[THETA] = (float) M_PI; // in radians
    res[RHO] = 1;
    res[THETA] = M_PI / 180;

    houghSpace linespace (res, maxVal);

    for (auto &p: readings) {
        for (float beta = -M_PI/12; beta <= M_PI/12; beta += M_PI/180) {
            vote[THETA] = p.theta + beta;
            if (vote[THETA] < 0) {
                vote[THETA] += M_PI;
            } else if (vote[THETA] > M_PI) {
                vote[THETA] -= M_PI;
            }
            vote[RHO] = p.reading + (p.x * cosf(vote[THETA]))
                                  + (p.y * sinf(vote[THETA]));
            assert(vote[RHO] <= maxVal[RHO]);
            assert(vote[THETA] <= maxVal[THETA]);
            linespace.addVote(vote);
        }
    }
    // TODO do for point space too
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
