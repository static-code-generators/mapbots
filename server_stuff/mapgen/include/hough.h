#pragma once

#include <vector>
#include <iostream>
#include <cassert>
#include <unordered_map>
#include <boost/functional/hash.hpp>

#define ROUND(x) (int)(x + 0.5)

namespace helper
{
    extern long long pow(long long a, unsigned int b);
}

namespace std
{
    template<>
        struct hash < vector<int> > 
        {
            size_t operator()(const vector<int> &T) const
            {
                size_t temp = 42;
                for (const int &i : T)
                    boost::hash_combine(temp, i);
                return temp;
            }
        };
}

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
    std::unordered_map<std::vector<int>, int> m_votingTable;
    int isMaxima(std::vector<int> idx);
    public:
    //void printVotingTable(std::ostream &str);
    houghSpace(std::vector<float> res, std::vector<float> maxVal, std::vector<float> minVal = std::vector<float>(2, 0));
    void addVote(std::vector<float> vote);
    std::vector< std::vector<float> > getMaxima(int threshold);
};

//std::ostream& operator<<(std::ostream &str, houghSpace &hs);
