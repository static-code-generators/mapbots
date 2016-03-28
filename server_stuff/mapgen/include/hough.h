#pragma once

#include "boost/multi_array.hpp"
#include "boost/math/special_functions/pow.hpp"
#include <vector>
#include <iostream>
#include <cassert>

const int numDim = 3;

typedef boost::multi_array<float, numDim> table;
typedef boost::array<table::index, numDim> tableIndices; // numDim-dimensional vector

#define ROUND(x) (int)(x + 0.5)

class houghSpace
{
    std::vector<float> m_res;
    std::vector<float> m_maxVal;
    std::vector<float> m_minVal;
    tableIndices m_shape;
    table m_votingTable; 
    void initShape();
    int isMaxima(tableIndices idx);
    int isMaximaEdgeOnly(tableIndices idx);
    tableIndices indexOf(std::vector<float> cell);
public:
    void printVotingTable(std::ostream &str);
    houghSpace(std::vector<float> res, std::vector<float> maxVal, std::vector<float> minVal = std::vector<float>(numDim, 0));
    void addVote(std::vector<float> vote);
    std::vector< std::vector<float> > getMaxima(int threshold);
};

table::index getIndex(const table& m, const float* requestedElement, const unsigned short int direction);
tableIndices getIndexArray(const table& m, const float* requestedElement);

//std::ostream& operator<<(std::ostream &str, houghSpace &hs);
