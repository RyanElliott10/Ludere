//
// Created by Ryan Elliott on 8/7/20.
//

#ifndef SUCCESSOR_IDATAEVENTSUBSCRIBER_HPP
#define SUCCESSOR_IDATAEVENTSUBSCRIBER_HPP


#include <unordered_map>
#include <unordered_set>

#include <ludere/CandlestickData.hpp>

namespace lud {

/// Defines what to implement to subscribe to a data stream.
struct IDataEventSubscriber
{
    virtual ~IDataEventSubscriber() = default;

    /// Called when the Exchange has received update data. Should be the main entry point for handling updated market
    /// data.
    virtual void handleMarketData(const std::unordered_map<std::string, lud::CandlestickData> &data) = 0;

    /// Set of securities a Strategy is interested in.
    std::unordered_set<std::string> m_subscribedSecurities;
};

/// Utilized to subscribe a Strategy to a data stream.
struct DataEventSubscription
{
    virtual ~DataEventSubscription() = default;

    DataEventSubscription(std::shared_ptr<IDataEventSubscriber> ref_, std::unordered_set<std::string> securities_)
            : ref(std::move(ref_)), securities(std::move(securities_))
    {}

    /// A reference to the Strategy being subscribed.
    std::shared_ptr<IDataEventSubscriber> ref;

    /// A list of Securities. At this point, strictly strings
    std::unordered_set<std::string> securities;
};

}


#endif //SUCCESSOR_IDATAEVENTSUBSCRIBER_HPP
