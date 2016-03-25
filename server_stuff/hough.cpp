#include "hough.h"


houghSpace::houghSpace() :
    m_res (numDim, 0),
    m_maxVal (numDim, 0)
{
    initShape();
    m_votingTable = table(m_shape);
    std::fill(m_votingTable.origin(), m_votingTable.origin() + m_votingTable.num_elements(), 0);
}

houghSpace::houghSpace(std::vector<float> res, std::vector<float> maxVal) :
    m_res (res),
    m_maxVal (maxVal)
{
    initShape();
    m_votingTable = table(m_shape);
    std::fill(m_votingTable.origin(), m_votingTable.origin() + m_votingTable.num_elements(), 0);
}

void houghSpace::initShape()
{
    for (int i = 0; i < numDim; ++i) {
        //+1 because 0-indexing
        m_shape[i] = ROUND(m_maxVal[i] / m_res[i]) + 1; 
    }
}

void houghSpace::addVote(std::vector<float> vote)
{
    boost::array<table::index, numDim> idx;
    for (int i = 0; i < numDim; ++i) {
        idx[i] = ROUND(vote[i] / m_res[i]);
        assert(idx[i] <= m_maxVal[i]);
    }
    m_votingTable(idx)++;
}
