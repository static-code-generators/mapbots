#pragma once

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

//Courtesy Loki Astari, http://lokiastari.com/

class CSVRow
{
    public:
        inline std::string const &operator[](std::size_t index) const
        {
            return m_data[index];
        }
        inline std::size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(std::istream &str);
        void writeRow(std::ostream &str) const;
    private:
        std::vector<std::string>    m_data;
};

std::istream& operator>>(std::istream &str, CSVRow &data);
std::ostream& operator<<(std::ostream &str, CSVRow &data);
