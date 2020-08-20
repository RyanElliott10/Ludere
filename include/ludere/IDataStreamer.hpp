//
// Created by Ryan Elliott on 8/14/20.
//

#ifndef SUCCESSOR_IDATASTREAMER_HPP
#define SUCCESSOR_IDATASTREAMER_HPP


#include <unordered_map>

#include <ludere/CandlestickData.hpp>

namespace lud {

using CandlestickDataMap = std::unordered_map<std::string, CandlestickData>;

struct IDataStreamer
{
    virtual ~IDataStreamer() = default;

    /// Called by an exchange to request the next data points.
    virtual CandlestickDataMap pollNextStream() = 0;
};

}


#endif //SUCCESSOR_IDATASTREAMER_HPP