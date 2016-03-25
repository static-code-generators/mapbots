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

int houghSpace::isMaxima(table_index idx)
{
    int indexDiff[3] = {-1, 0, 1};
    long long bound = boost::math::pow<numDim>(3);
    table_index neighbour = idx;
    //iterate over all neighbours
    //this makes sense because all neighbours have at most numDim indices
    //different with a diff in range {-1, 0, 1} which has size 3
    //please email parth15069@iiitd.ac.in if this doesn't make sense
    for(long long incr = 0; incr < bound; ++incr) {
        long long temp = incr;
        int safe = 1;
        for (int i = 0; i < numDim; ++i) {
            neighbour[i] = idx[i] + indexDiff[temp % 3]; 
            if (neighbour[i] >= m_shape[i] || neighbour[i] < 0)
                safe = 0;
            temp /= 3;
        }
        if (safe && neighbour != idx && m_votingTable(neighbour) >= m_votingTable(idx))
            return 0;
    }
    return 1;
}

void houghSpace::addVote(std::vector<float> vote)
{
    table_index idx;
    for (int i = 0; i < numDim; ++i) {
        idx[i] = ROUND(vote[i] / m_res[i]);
        assert(idx[i] <= m_maxVal[i]);
    }
    m_votingTable(idx)++;
}

table::index getIndex(const table& m, const float* requestedElement, const unsigned short int direction)
{
    int offset = requestedElement - m.origin();
    return(offset / m.strides()[direction] % m.shape()[direction] +  m.index_bases()[direction]); 
}

table_index getIndexArray(const table& m, const float* requestedElement)
{
    table_index _idx;
    for (unsigned int dir = 0; dir < numDim; dir++)
    {
        _idx[dir] = getIndex(m, requestedElement, dir);
    }
    return _idx;
}


std::vector< std::vector<float> > houghSpace::getMaxima(int threshold)
{
    std::vector< std::vector<float> > maxima;
    std::vector<float> currMaxima;
    auto p = m_votingTable.data();
    for (int i = 0; i < m_votingTable.num_elements(); ++i) {
        table_index idx = getIndexArray(m_votingTable, p);
        if(isMaxima(idx) && m_votingTable(idx) >= threshold) {
            for (int i = 0; i < numDim; ++i) {
                currMaxima[i] = idx[i] * m_res[i];
            }
            maxima.push_back(currMaxima);
        }
    }
    return maxima;
}
