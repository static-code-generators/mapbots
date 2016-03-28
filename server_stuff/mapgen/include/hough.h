#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <cassert>
#include <algorithm>

typedef std::pair< int, std::vector<int> > maximaType;

#define ROUND(x) (int)(x + 0.5)

namespace helper
{
    extern long long pow(long long a, unsigned int b);
}

struct maximaCompare
{
    bool operator()(const maximaType &lhs, const maximaType &rhs)
    {
        return std::tie(lhs.first, lhs.second) > std::tie(rhs.first, rhs.second);
    }
};

class houghSpace
{
    unsigned int m_numDim;
    std::vector<float> m_res;
    std::vector<float> m_maxVal;
    std::vector<float> m_minVal;
    std::vector<int> m_indexRange; //for sanity checking indices calculated
    std::vector<int> indexOf(std::vector<float> cell);
    std::vector<float> cellOf(std::vector<int> idx);
    //return a vector of neighbours which are safe to access
    std::vector< std::vector<int> > neighbours(std::vector<int>);
    std::map<std::vector<int>, int> m_votingTable;
    std::set<maximaType, maximaCompare> m_maxima;
    int isMaxima(std::vector<int> idx);
    public:
    //void printVotingTable(std::ostream &str);
    houghSpace(std::vector<float> res, std::vector<float> maxVal, std::vector<float> minVal = std::vector<float>(2, 0));
    void addVote(std::vector<float> vote);
    std::vector< std::vector<float> > getMaxima(int threshold);
};

//std::ostream& operator<<(std::ostream &str, houghSpace &hs);
