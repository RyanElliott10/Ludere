//
// Created by Ryan Elliott on 8/14/20.
//

#ifndef SUCCESSOR_DATASTREAMABLE_HPP
#define SUCCESSOR_DATASTREAMABLE_HPP


#include <unordered_map>

#include <ludere/candlestick_data.hpp>

namespace lud {

/**
 * TODO: Convert this into a class or struct to support metadata (like timestamp, etc.) in a far more elegant fashion
 *      than querying the map for "timestamp"
 */
using candlestick_data_map = std::unordered_map<std::string, candlestick_data>;

struct datastreamable
{
    virtual ~datastreamable() = default;

    /// Called by an exchange to request the next data points.
    virtual candlestick_data_map poll_next_stream() = 0;
};

}


#endif //SUCCESSOR_DATASTREAMABLE_HPP
