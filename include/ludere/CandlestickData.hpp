//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_CANDLESTICKDATA_HPP
#define LUDERE_CANDLESTICKDATA_HPP

#include <charconv>
#include <cstdint>
#include <iostream>
#include <vector>

#include <ludere/Log.hpp>

namespace lud {

enum class CSVRowSuccessReturnCode
{
    kSuccess = 0,
    kFailure = 1
};

enum class CandlestickDataCSVHeaders
{
    kTimestamp = 0,
    kOpen = 1,
    kHigh = 2,
    kLow = 3,
    kClose = 4,
    kVolume = 5
};

class CandlestickData
{
public:
    std::string ticker = "AAPL";
    float open;
    float high;
    float low;
    float close;
    uint32_t volume;

    // TODO: Add support for converting string timestamps into time_t (UNIX)
    std::string timestamp;

    std::string_view operator[](std::size_t index) const
    {
        return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] - (m_data[index] + 1));
    }

    std::size_t size() const
    {
        return m_data.size() - 1;
    }

    CSVRowSuccessReturnCode readNextRow(std::istream &str)
    {
        std::getline(str, m_line);

        m_data.clear();
        m_data.emplace_back(-1);
        std::string::size_type pos = 0;
        while ((pos = m_line.find(',', pos)) != std::string::npos) {
            m_data.emplace_back(pos);
            ++pos;
        }

        pos = m_line.size();
        m_data.emplace_back(pos);

        for (int i = 0; i < m_data.size() - 1; i++) {
            std::string tmp = m_line.substr(m_data[i] + 1, m_data[i + 1] - (m_data[i] + 1));
            // Detect header rows
            if (i > 0 && !isNumber(tmp)) {
                LD_WARN("Error parsing CSV row (potentially header. If so, disregard this message): %s", tmp.c_str());
                return CSVRowSuccessReturnCode::kFailure;
            }

            switch ((CandlestickDataCSVHeaders) i) {
            case CandlestickDataCSVHeaders::kTimestamp:
                timestamp = tmp;
                break;
            case CandlestickDataCSVHeaders::kOpen:
                open = std::stof(tmp);
                break;
            case CandlestickDataCSVHeaders::kHigh:
                high = std::stof(tmp);
                break;
            case CandlestickDataCSVHeaders::kLow:
                low = std::stof(tmp);
                break;
            case CandlestickDataCSVHeaders::kClose:
                close = std::stof(tmp);
                break;
            case CandlestickDataCSVHeaders::kVolume:
                volume = std::stoi(tmp);
                break;
            }
        }

        return CSVRowSuccessReturnCode::kSuccess;
    }

private:
    std::string m_line;
    std::vector<int> m_data;

private:
    inline static bool isNumber(std::string &str)
    {
        return !str.empty() && std::all_of(str.begin(), str.end(), CandlestickData::isdigit);
    }

    inline static bool isdigit(const char c)
    {
        return c == '.' || std::isdigit(c);
    }
};

inline std::ostream &operator<<(std::ostream &strm, const CandlestickData &candle)
{
    return strm << candle.timestamp << " " << candle.open << " " << candle.high << " " << candle.low << " "
                << candle.close << " " << candle.volume;
}

inline std::istream &operator>>(std::istream &strm, CandlestickData &candle)
{
    auto status = candle.readNextRow(strm);
    if (status == CSVRowSuccessReturnCode::kFailure) {
        strm >> candle;
    }
    return strm;
}

}


#endif //LUDERE_CANDLESTICKDATA_HPP
