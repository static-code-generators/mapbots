#include "houghalt.h"

namespace helper
{
    long long pow(long long a, unsigned int b)
    {
        long long ans = 1, res = a;
        while (b > 0) {
            if (b % 2)
                ans *= res;
            res *= res;
            b /= 2;
        }
        return ans;
    }
}

houghSpace::houghSpace(std::vector<float> res, std::vector<float> maxVal, std::vector<float> minVal) :
    m_res (res),
    m_maxVal (maxVal),
    m_minVal (minVal)
{
    m_numDim = res.size();
    if(m_minVal.size() != m_numDim)
        m_minVal.resize(m_numDim, 0);
    m_indexRange.resize(m_numDim, 0);
    for (int i = 0; i < m_numDim; ++i)
        m_indexRange[i] = std::lround((m_maxVal[i] - m_minVal[i]) / m_res[i]);
}

std::vector<int> houghSpace::indexOf(std::vector<float> cell)
{
    std::vector<int> idx(m_numDim, 0);
    for (int i = 0; i < m_numDim; ++i) {
        assert(cell[i] >= m_minVal[i] && cell[i] <= m_maxVal[i]);
        idx[i] = std::lround((cell[i] - m_minVal[i]) / m_res[i]);
        assert(idx[i] <= m_indexRange[i] && idx[i] >= 0);
    }
    return idx;
}

std::vector<float> houghSpace::cellOf(std::vector<int> idx)
{
    std::vector<float> cell(m_numDim, 0);
    for (int i = 0; i < m_numDim; ++i) {
        cell[i] = m_minVal[i] + idx[i] * m_res[i];
    }
    return cell;
}

std::vector< std::vector<int> > houghSpace::neighbours(std::vector<int> idx)
{
    //iterate over all neighbours
    //this makes sense because all neighbours have at most m_numDim indices
    //different with a diff in range {-1, 0, 1} which has size 3
    //please email parth15069@iiitd.ac.in if this doesn't make sense
    int indexDiff[3] = {-1, 0, 1};
    long long bound = helper::pow(3, m_numDim);
    std::vector<int> neighbour(m_numDim, 0);
    std::vector< std::vector<int> > ans;
    for(long long incr = 0; incr < bound; ++incr) {
        long long temp = incr;
        int safe = 1;
        for (int i = 0; i < m_numDim; ++i) {
            neighbour[i] = idx[i] + indexDiff[temp % 3]; 
            if (neighbour[i] > m_indexRange[i] || neighbour[i] < 0)
                safe = 0;
            temp /= 3;
        }
        if (safe && !(neighbour == idx))
            ans.push_back(neighbour);
    }
    return ans;
}

int houghSpace::isMaxima(std::vector<int> idx)
{
    for (auto &neighbour : neighbours(idx))
        if (m_votingTable[neighbour].size() >= m_votingTable[idx].size()) 
            return 0;
    return 1;
}

void houghSpace::addVote(std::vector<float> vote, std::vector<float> loc)
{
    auto idx = indexOf(vote);
    m_votingTable[idx].push_back(loc);
}

std::vector< std::pair< std::vector<float>, std::vector< std::vector<float> > > > houghSpace::getMaxima(int threshold)
{
    std::vector< std::pair< std::vector<float>, std::vector< std::vector<float> > > > maxima;
    for (auto &cand : m_votingTable)
        if (cand.second.size() >= threshold && isMaxima(cand.first))
            maxima.push_back(make_pair(cellOf(cand.first), cand.second));
    return maxima;
}
