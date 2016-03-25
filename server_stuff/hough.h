#pragma once

#include <vector>
#include "boost/multi_array.hpp"
#include <cassert>

const int numDim = 2;

typedef boost::multi_array<float, numDim> table;

#define ROUND(x) (int)(x + 0.5)

class houghSpace
{
    std::vector<float> m_res;
    std::vector<float> m_maxVal;
    boost::array<table::index, numDim> m_shape;
    table m_votingTable; 
    void initShape();
public:
    houghSpace();
    houghSpace(std::vector<float> res, std::vector<float> maxVal);
    void addVote(std::vector<float> vote);
};
