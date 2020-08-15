//
// Created by Ryan Elliott on 8/1/20.
//

#ifndef SUCCESSOR_CSV_HPP
#define SUCCESSOR_CSV_HPP


#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

namespace lud {

class CSVRow
{
public:
    typedef std::string_view iterator;
    typedef const std::string_view const_iterator;

    std::string_view operator[](std::size_t index) const
    {
        return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] - (m_data[index] + 1));
    }

    [[nodiscard]] std::size_t size() const
    {
        return m_data.size() - 1;
    }

    void readNextRow(std::istream &str)
    {
        std::getline(str, m_line);

        m_data.clear();
        m_data.emplace_back(-1);
        std::string::size_type pos = 0;
        while ((pos = m_line.find(',', pos)) != std::string::npos) {
            m_data.emplace_back(pos);
            ++pos;
        }
        // This checks for a trailing comma with no data after it.
        pos = m_line.size();
        m_data.emplace_back(pos);
    }

private:
    std::string m_line;
    std::vector<int> m_data;
};

inline std::istream &operator>>(std::istream &str, CSVRow &data)
{
    data.readNextRow(str);
    return str;
}

class CSVIterator
{
public:
    typedef std::input_iterator_tag iterator_category;
    typedef CSVRow value_type;
    typedef std::size_t difference_type;
    typedef CSVRow *pointer;
    typedef CSVRow &reference;

    explicit CSVIterator(std::istream &str)
            : m_str(str.good() ? &str : nullptr)
    {
        ++(*this);
    }

    CSVIterator()
            : m_str(nullptr)
    {}

    // Pre Increment
    inline CSVIterator &operator++()
    {
        if (m_str) {
            if (!((*m_str) >> m_row)) {
                m_str = nullptr;
            }
        }
        return *this;
    }

    // Post increment
    inline CSVIterator operator++(int)
    {
        CSVIterator tmp(*this);
        ++(*this);
        return tmp;
    }

    inline CSVRow const &operator*() const
    {
        return m_row;
    }

    inline CSVRow const *operator->() const
    {
        return &m_row;
    }

    inline bool operator==(CSVIterator const &rhs)
    {
        return ((this == &rhs) || ((this->m_str == nullptr) && (rhs.m_str == nullptr)));
    }

    inline bool operator!=(CSVIterator const &rhs)
    {
        return !(*this == rhs);
    }

private:
    std::istream *m_str;
    CSVRow m_row;
};

}


#endif //SUCCESSOR_CSV_HPP
