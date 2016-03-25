//#pragma once

#include <vector>

#define numDim 2

class houghSpace
{
    std::vector<float> m_res;
    std::vector<float> m_maxVal;
    std::vector< std::vector<int> > m_voteTable;
public:
    houghSpace() :
        m_res (2, 0),
        m_maxVal (2, 0)
    {
    }
    houghSpace(std::vector<float> res, std::vector<float> maxVal) :
        m_res (res),
        m_maxVal (maxVal)
    {
    }
};
