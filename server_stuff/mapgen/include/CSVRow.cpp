#include "CSVRow.h"

void CSVRow::readNextRow(std::istream &str)
{
    std::string         line;
    std::getline(str,line);

    std::stringstream   lineStream(line);
    std::string         cell;

    m_data.clear();
    while(std::getline(lineStream,cell,',')) {
        m_data.push_back(cell);
    }
}

void CSVRow::writeRow(std::ostream &str) const
{
    auto back = m_data.end() - 1;
    for(auto el = m_data.begin(); el != back; ++el) {
        str << *el << ',';
    }
    str << m_data.back();
}

std::istream& operator>>(std::istream &str, CSVRow &data)
{
    data.readNextRow(str);
    return str;
}   

std::ostream& operator<<(std::ostream &str, CSVRow &data)
{
    data.writeRow(str);
    return str;
}
