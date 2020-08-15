//
// Created by Ryan Elliott on 8/7/20.
//

#ifndef SUCCESSOR_IDATAEVENTSUBSCRIBER_HPP
#define SUCCESSOR_IDATAEVENTSUBSCRIBER_HPP


#include <unordered_set>

#include <ludere/CandlestickData.hpp>

namespace lud {

struct IDataEventSubscriber
{
    virtual ~IDataEventSubscriber() = default;

    virtual void handleMarketData(lud::CandlestickData &data) = 0;

    std::unordered_set<std::string> m_subscribedSecurities;
};

// Used to subscribe a Strategy
struct DataEventSubscription
{
    virtual ~DataEventSubscription() = default;
    DataEventSubscription() = default;

    DataEventSubscription(std::shared_ptr<IDataEventSubscriber> ref_, std::unordered_set<std::string> &securities_)
            : ref(std::move(ref_)), securities(securities_)
    {}

    std::shared_ptr<IDataEventSubscriber> ref;
    std::unordered_set<std::string> securities;
};

}


#endif //SUCCESSOR_IDATAEVENTSUBSCRIBER_HPP
