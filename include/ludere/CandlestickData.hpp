//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_CANDLESTICKDATA_HPP
#define LUDERE_CANDLESTICKDATA_HPP

#include <charconv>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>

#include <ludere/Log.hpp>
#include <ludere/UUID.hpp>

namespace lud {

enum class ParseCSVRowReturnCode
{
    kSuccess = 0,
    kFailure = 1,
    kEmpty = 2
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
    CandlestickData(std::string ticker_, time_t timestamp_, float open_, float high_, float low_, float close_,
                    uint32_t volume_)
            : ticker(std::move(ticker_)), timestamp(timestamp_), open(open_), high(high_), low(low_), close(close_),
              volume(volume_)
    {}

    std::string ticker;
    float open;
    float high;
    float low;
    float close;
    time_t timestamp;
    uint32_t volume;
    UUID uuid = UUID();

    bool operator==(const CandlestickData &candle) const
    {
        return timestamp == candle.timestamp && ticker == candle.ticker;
    }
};

inline std::ostream &operator<<(std::ostream &strm, const CandlestickData &candle)
{
    return strm << candle.ticker << " ts: " << candle.timestamp << " open: " << candle.open << " high: " << candle.high
                << " low: " << candle.low << " close: " << candle.close << " vol: " << candle.volume;
}

}

namespace std {

template<>
struct hash<lud::CandlestickData>
{
    inline size_t operator()(const lud::CandlestickData &candle) const
    {
        return hash<int>()(candle.uuid.hash());
    }
};

}


#endif //LUDERE_CANDLESTICKDATA_HPP
