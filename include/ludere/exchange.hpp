//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EXCHANGE_HPP
#define LUDERE_EXCHANGE_HPP


#include <list>
#include <vector>

#include <ludere/core.hpp>
#include <ludere/data_event_subscribable.hpp>
#include <ludere/data_streamable.hpp>
#include <ludere/market_event_subscribable.hpp>
#include <ludere/order_event.hpp>

namespace lud {

class exchange
{
public:
    explicit exchange(std::shared_ptr<data_streamable> data_stream_);
    exchange(std::shared_ptr<data_streamable> data_stream_, bool inverted_data_stream_);

    /// Initiates trading on the exchange and informs all Strategies trading has begun.
    void trade();

    /// Handles queuing of events.
    void addEvent(std::shared_ptr<event> &event_);

    /// Used to subscribe a Strategy to an exchange's datastream.
    void subscribe_to_data_stream(const std::shared_ptr<data_event_subscription> &subscription_);

private:
    bool m_is_trading;
    event_queue m_event_queue;
    std::list<std::shared_ptr<order_event>> m_order_queue;
    const bool m_inverted_data_stream = false;
    std::shared_ptr<data_streamable> m_dataStream;

    // Subscribers
    std::vector<const std::shared_ptr<data_event_subscription>> m_data_event_subscribers;
    std::vector<const std::shared_ptr<market_event_subscribable>> m_market_event_subscribers;

private:
    void stream_data(const candlestick_data_map &data_);
    void handle_order_event(const std::shared_ptr<event> &event_, const candlestick_data_map &candles_);
    void handle_market_event(const std::shared_ptr<event> &event_);
    void fill_orders(const candlestick_data_map &candles_);
    void handle_limit_order(const std::shared_ptr<limit_order> &order_, const candlestick_data_map &candles_, std::list<std::shared_ptr<order_event>>::iterator &it_);
    void handle_market_order(const std::shared_ptr<market_order> &order_, const candlestick_data_map &candles_, std::list<std::shared_ptr<order_event>>::iterator &it_);
};

}


#endif //LUDERE_EXCHANGE_HPP
