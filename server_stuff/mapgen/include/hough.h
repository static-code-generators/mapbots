#pragma once

#include "boost/multi_array.hpp"
#include "boost/math/special_functions/pow.hpp"
#include <vector>
#include <iostream>
#include <cassert>

const int numDim = 2;

typedef boost::multi_array<float, numDim> table;
typedef boost::array<table::index, numDim> tableIndices;

#define ROUND(x) (int)(x + 0.5)

class houghSpace
{
    std::vector<float> m_res;
    std::vector<float> m_maxVal;
    tableIndices m_shape;
    table m_votingTable; 
    void initShape();
    int isMaxima(tableIndices idx);
public:
    void printVotingTable(std::ostream &str);
    houghSpace(std::vector<float> res, std::vector<float> maxVal);
    void addVote(std::vector<float> vote);
    std::vector< std::vector<float> > getMaxima(int threshold);
};

table::index getIndex(const table& m, const float* requestedElement, const unsigned short int direction);
tableIndices getIndexArray(const table& m, const float* requestedElement);

std::ostream& operator<<(std::ostream &str, houghSpace &hs);