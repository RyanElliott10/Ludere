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

#include <ludere/log.hpp>
#include <ludere/uuid.hpp>

namespace lud {

enum class parse_csv_row_return_status
{
    SUCCESS = 0,
    FAILURE = 1,
    EMPTY = 2
};

enum class candlestick_data_csv_headers
{
    TIMESTAMP = 0,
    OPEN = 1,
    HIGH = 2,
    LOW = 3,
    CLOSE = 4,
    VOLUME = 5
};

class candlestick_data
{
public:
    candlestick_data(std::string ticker_, time_t timestamp_, float open_, float high_, float low_, float close_,
                     uint32_t volume_);

    explicit candlestick_data(const std::string ticker_, time_t timestamp_);

    std::string m_ticker;
    float m_open;
    float m_high;
    float m_low;
    float m_close;
    time_t m_timestamp;
    uint32_t m_volume;
    uuid m_uuid = uuid();

    bool operator==(const candlestick_data &candle_) const;
};

}


#endif //LUDERE_CANDLESTICKDATA_HPP
