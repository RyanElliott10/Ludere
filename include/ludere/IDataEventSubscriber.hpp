//
// Created by Ryan Elliott on 8/7/20.
//

#ifndef SUCCESSOR_IDATAEVENTSUBSCRIBER_HPP
#define SUCCESSOR_IDATAEVENTSUBSCRIBER_HPP


#include <ludere/CandlestickData.hpp>

namespace lud {

struct IDataEventSubscriber
{
    virtual ~IDataEventSubscriber() = default;

    virtual void handleMarketData(lud::CandlestickData &data) = 0;
};

}


#endif //SUCCESSOR_IDATAEVENTSUBSCRIBER_HPP
