//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EXCHANGE_HPP
#define LUDERE_EXCHANGE_HPP


#include <list>
#include <vector>

#include <ludere/core.hpp>
#include <ludere/data_event_subscribable.hpp>
#include <ludere/datastreamable.hpp>
#include <ludere/limit_order.hpp>
#include <ludere/market_order.hpp>
#include <ludere/market_event_subscribable.hpp>
#include <ludere/order_event.hpp>

namespace lud {

class exchange
{
public:
    explicit exchange(std::shared_ptr<datastreamable> datastream_);
    exchange(std::shared_ptr<datastreamable> datastream_, bool inverted_datastream_);

    /// Initiates trading on the exchange and informs all Strategies trading has begun.
    void trade();

    /// Handles queuing of events.
    void add_event(std::shared_ptr<event> &event_);

    /// Used to subscribe a Strategy to an exchange's datastream.
    void subscribe_to_datastream(const std::shared_ptr<data_event_subscription> &subscription_);

private:
    bool m_is_trading;
    event_queue m_event_queue;
    std::list<std::shared_ptr<order_event>> m_order_queue;
    const bool m_inverted_datastream = false;
    std::shared_ptr<datastreamable> m_datastream;

    // Subscribers
    std::vector<const std::shared_ptr<data_event_subscription>> m_data_event_subscribers;
    std::vector<const std::shared_ptr<market_event_subscribable>> m_market_event_subscribers;

private:
    void stream_data(const candlestick_data_aggregate &data_);
    void handle_order_event(const std::shared_ptr<event> &event_, const candlestick_data_aggregate &candles_);
    void handle_market_event(const std::shared_ptr<event> &event_);
    void fill_orders(const candlestick_data_aggregate &candles_);

    // Order handling
    void handle_limit_order(const std::shared_ptr<limit_order> &order_, const candlestick_data_aggregate &candles_,
                            std::list<std::shared_ptr<order_event>>::iterator &it_);
    void handle_buy_limit_order(const std::shared_ptr<limit_order> &order_, const candlestick_data_aggregate &candles_,
                                std::list<std::shared_ptr<order_event>>::iterator &it_);
    void handle_sell_limit_order(const std::shared_ptr<limit_order> &order_, const candlestick_data_aggregate &candles_,
                                 std::list<std::shared_ptr<order_event>>::iterator &it_);

    void handle_market_order(const std::shared_ptr<market_order> &order_, const candlestick_data_aggregate &candles_,
                             std::list<std::shared_ptr<order_event>>::iterator &it_);
    void
    handle_buy_market_order(const std::shared_ptr<market_order> &order_, const candlestick_data_aggregate &candles_,
                            std::list<std::shared_ptr<order_event>>::iterator &it_);
    void
    handle_sell_market_order(const std::shared_ptr<market_order> &order_, const candlestick_data_aggregate &candles_,
                             std::list<std::shared_ptr<order_event>>::iterator &it_);
};

}


#endif //LUDERE_EXCHANGE_HPP
