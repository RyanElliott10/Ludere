//
// Created by Ryan Elliott on 8/14/20.
//

#ifndef SUCCESSOR_DATA_STREAMABLE_HPP
#define SUCCESSOR_DATA_STREAMABLE_HPP


#include <unordered_map>

#include <ludere/candlestick_data.hpp>

namespace lud {

using candlestick_data_map = std::unordered_map<std::string, candlestick_data>;

struct data_streamable
{
    virtual ~data_streamable() = default;

    /// Called by an exchange to request the next data points.
    virtual candlestick_data_map poll_next_stream() = 0;
};

}


#endif //SUCCESSOR_DATA_STREAMABLE_HPP
