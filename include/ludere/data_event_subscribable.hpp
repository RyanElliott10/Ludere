//
// Created by Ryan Elliott on 8/7/20.
//

#ifndef SUCCESSOR_DATA_EVENT_SUBSCRIBABLE_HPP
#define SUCCESSOR_DATA_EVENT_SUBSCRIBABLE_HPP


#include <unordered_map>
#include <unordered_set>

#include <ludere/candlestick_data.hpp>

namespace lud {

struct candlestick_data_aggregate;

/// Defines what to implement to subscribe to a data stream.
struct data_event_subscribable
{
    virtual ~data_event_subscribable() = default;

    /// Called when the exchange has received update data. Should be the main entry point for handling updated market
    /// data.
    virtual void handle_market_data(const lud::candlestick_data_aggregate &data_) = 0;

    /// Set of securities a Strategy is interested in.
    std::unordered_set<std::string> m_subscribed_securities;
};

/// Utilized to subscribe a Strategy to a data stream.
struct data_event_subscription
{
    virtual ~data_event_subscription() = default;

    data_event_subscription(std::shared_ptr<data_event_subscribable> ref_, std::unordered_set<std::string> securities_)
            : m_ref(std::move(ref_)), m_securities(std::move(securities_))
    {}

    /// A reference to the Strategy being subscribed.
    std::shared_ptr<data_event_subscribable> m_ref;

    /// A list of Securities. At this point, strictly strings
    std::unordered_set<std::string> m_securities;
};

}


#endif //SUCCESSOR_DATA_EVENT_SUBSCRIBABLE_HPP
