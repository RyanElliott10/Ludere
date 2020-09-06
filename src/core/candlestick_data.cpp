//
// Created by Ryan Elliott on 9/6/20.
//

#include <ludere/candlestick_data.hpp>

namespace lud {

candlestick_data::candlestick_data(const std::string ticker_, const time_t timestamp_, float open_, float high_,
                                   float low_, float close_, uint32_t volume_)
        : m_ticker(std::move(ticker_)), m_timestamp(timestamp_), m_open(open_), m_high(high_), m_low(low_),
          m_close(close_),
          m_volume(volume_)
{}

candlestick_data::candlestick_data(const std::string ticker_, const time_t timestamp_)
        : m_ticker(std::move(ticker_)), m_timestamp(timestamp_)
{}

bool candlestick_data::operator==(const candlestick_data &candle_) const
{
    return m_timestamp == candle_.m_timestamp && m_ticker == candle_.m_ticker;
}

inline std::ostream &operator<<(std::ostream &strm_, const candlestick_data &candle_)
{
    return strm_ << candle_.m_ticker << " ts: " << candle_.m_timestamp << " open: " << candle_.m_open << " high: "
                 << candle_.m_high
                 << " low: " << candle_.m_low << " close: " << candle_.m_close << " vol: " << candle_.m_volume;
}

}

namespace std {

template<>
struct hash<lud::candlestick_data>
{

    __attribute__((always_inline))
    size_t operator()(const lud::candlestick_data &candle_) const
    {
        return hash<int>()(candle_.m_uuid.hash());
    }

};

}