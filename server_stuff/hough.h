#pragma once

#include <vector>
#include "boost/multi_array.hpp"
#include "boost/math/special_functions/pow.hpp"
#include <cassert>

const int numDim = 2;

typedef boost::multi_array<float, numDim> table;
typedef boost::array<table::index, numDim> table_index;

#define ROUND(x) (int)(x + 0.5)

class houghSpace
{
    std::vector<float> m_res;
    std::vector<float> m_maxVal;
    table_index m_shape;
    table m_votingTable; 
    void initShape();
    int isMaxima(table_index idx);
public:
    houghSpace();
    houghSpace(std::vector<float> res, std::vector<float> maxVal);
    void addVote(std::vector<float> vote);
    std::vector< std::vector<float> > getMaxima(int threshold);
};

table::index getIndex(const table& m, const float* requestedElement, const unsigned short int direction);
table_index getIndexArray(const table& m, const float* requestedElement);
