//
// Created by Ryan Elliott on 7/30/20.
//

#include <utility>

#include <ludere/candlestick_data.hpp>
#include <ludere/exchange.hpp>
#include <ludere/filled_order.hpp>


namespace lud {

exchange::exchange(std::shared_ptr<datastreamable> datastream_)
        : m_datastream(std::move(datastream_)), m_is_trading(false)
{}

exchange::exchange(std::shared_ptr<datastreamable> datastream_, const bool inverted_datastream_)
        : m_datastream(std::move(datastream_)), m_inverted_datastream(inverted_datastream_), m_is_trading(false)
{
    if (m_inverted_datastream) {
        LUD_ERROR("Inverted datastream is not yet supported", nullptr);
        exit(-1);
    }
}

void exchange::trade()
{
    m_is_trading = true;

    candlestick_data_map candles_;
    while (!(candles_ = m_datastream->poll_next_stream()).empty()) {
        stream_data(candles_);

        while (!m_event_queue.empty()) {
            std::shared_ptr<event> event_ = m_event_queue.front();
            switch (event_->m_type) {
            case event_type::ORDER:
                handle_order_event(event_, candles_);
                break;
            case event_type::MARKET:
                handle_market_event(event_);
                break;
            }

            m_event_queue.pop();
        }

        fill_orders(candles_);
    }
}

void exchange::stream_data(const candlestick_data_map &data_)
{
    for (const auto &subscriber_ : m_data_event_subscribers) {
        subscriber_->m_ref->handle_market_data(data_);
    }
}

void exchange::handle_order_event(const std::shared_ptr<event> &event_, const candlestick_data_map &candles_)
{
    std::shared_ptr<order_event> order_event_ = std::dynamic_pointer_cast<order_event>(event_);
    m_order_queue.push_back(order_event_);
}

void exchange::fill_orders(const candlestick_data_map &candles_)
{
    auto it_ = m_order_queue.begin();
    while (it_ != m_order_queue.end()) {
        // TODO: Add metadata about the candles to avoid accessing a specific security (timestamp)
        if ((*it_)->m_order->is_expired(candles_.at("timestamp").m_timestamp)) {
            const std::shared_ptr<limit_order> &order_ = std::dynamic_pointer_cast<limit_order>((*it_)->m_order);
            std::unique_ptr<filled_order> filled_order_ = std::make_unique<filled_order>(order_->m_security,
                                                                                         order_->m_num_shares,
                                                                                         0,
                                                                                         enums::order::signals::SELL,
                                                                                         enums::order::fill_statuses::EXPIRED,
                                                                                         candles_.at(
                                                                                                 "timestamp").m_timestamp,
                                                                                         order_->m_uuid);
            (*it_)->m_callback(std::move(filled_order_));
            it_ = m_order_queue.erase(it_);
            continue;
        }
        enums::order::types &type_ = (*it_)->m_order->m_order_type;
        // TODO: This is broken. If an order isn't filled, the iterator is never incremented. Control of the iterator
        //      for incrementing should be in the hands of this function.
        if (type_ == enums::order::types::LIMIT) {
            handle_limit_order(std::dynamic_pointer_cast<limit_order>((*it_)->m_order), candles_, it_);
        } else if (type_ == enums::order::types::MARKET) {
            handle_market_order(std::dynamic_pointer_cast<market_order>((*it_)->m_order), candles_, it_);
        } else {
            it_++;
        }
    }
}

// TODO: Implement slippage and more realistic market conditions. This is guaranteed to fill your order at the best
//      price and immediately (as long as the security's price is below the limit)
void exchange::handle_limit_order(const std::shared_ptr<limit_order> &order_, const candlestick_data_map &candles_,
                                  std::list<std::shared_ptr<order_event>>::iterator &it_)
{
    switch (order_->m_order_signal) {
    case enums::order::signals::BUY:
        handle_buy_limit_order(order_, candles_, it_);
        break;
    case enums::order::signals::SELL:
        handle_sell_limit_order(order_, candles_, it_);
        break;
    }
}

/**
 * Typical MarketOrder handling: the order is filled at the highest market price.
 * TODO: Place on the top of the order queue. Will require using a PriorityQueue rather than a list, or can add
 *      additional logic.
 */
void exchange::handle_market_order(const std::shared_ptr<market_order> &order_, const candlestick_data_map &candles_,
                                   std::list<std::shared_ptr<order_event>>::iterator &it_)
{
    switch (order_->m_order_signal) {
    case enums::order::signals::BUY:
        handle_buy_market_order(order_, candles_, it_);
        break;
    case enums::order::signals::SELL:
        handle_sell_market_order(order_, candles_, it_);
        break;
    }
}

void exchange::handle_buy_limit_order(const std::shared_ptr<limit_order> &order_, const candlestick_data_map &candles_,
                                      std::list<std::shared_ptr<order_event>, std::allocator<std::shared_ptr<order_event>>>::iterator &it_)
{
    const float security_price_ = candles_.at(order_->m_security).m_close;
    if (order_->m_limit_price >= security_price_ &&
        (*it_)->m_verify_portfolio_funds(order_->m_num_shares * security_price_)) {
        std::unique_ptr<filled_order> filled_order_ = std::make_unique<filled_order>(order_->m_security,
                                                                                     order_->m_num_shares,
                                                                                     security_price_,
                                                                                     order_->m_order_signal,
                                                                                     enums::order::fill_statuses::SUCCESS,
                                                                                     candles_.at(
                                                                                             order_->m_security).m_timestamp,
                                                                                     order_->m_uuid);
        (*it_)->m_callback(std::move(filled_order_));
        it_ = m_order_queue.erase(it_);
    } else { it_++; }
}

void exchange::handle_sell_limit_order(const std::shared_ptr<limit_order> &order_, const candlestick_data_map &candles_,
                                       std::list<std::shared_ptr<order_event>, std::allocator<std::shared_ptr<order_event>>>::iterator &it_)
{
    const float security_price_ = candles_.at(order_->m_security).m_close;
    if (security_price_ >= order_->m_limit_price &&
        (*it_)->m_verify_portfolio_shares(order_->m_num_shares, order_->m_security)) {
        std::unique_ptr<filled_order> filled_order_ = std::make_unique<filled_order>(order_->m_security,
                                                                                     order_->m_num_shares,
                                                                                     security_price_,
                                                                                     order_->m_order_signal,
                                                                                     enums::order::fill_statuses::SUCCESS,
                                                                                     candles_.at(
                                                                                             order_->m_security).m_timestamp,
                                                                                     order_->m_uuid);
        (*it_)->m_callback(std::move(filled_order_));
        it_ = m_order_queue.erase(it_);
    } else { it_++; }
}

void
exchange::handle_buy_market_order(const std::shared_ptr<market_order> &order_, const candlestick_data_map &candles_,
                                  std::list<std::shared_ptr<order_event>, std::allocator<std::shared_ptr<order_event>>>::iterator &it_)
{
    const float security_price_ = candles_.at(order_->m_security).m_close;
    if ((*it_)->m_verify_portfolio_funds(order_->m_num_shares * security_price_)) {
        std::unique_ptr<filled_order> filled_order_ = std::make_unique<filled_order>(order_->m_security,
                                                                                     order_->m_num_shares,
                                                                                     security_price_,
                                                                                     order_->m_order_signal,
                                                                                     enums::order::fill_statuses::SUCCESS,
                                                                                     candles_.at(
                                                                                             (*it_)->m_order->m_security).m_timestamp,
                                                                                     order_->m_uuid);
        (*it_)->m_callback(std::move(filled_order_));
        it_ = m_order_queue.erase(it_);
    }
}

void
exchange::handle_sell_market_order(const std::shared_ptr<market_order> &order_, const candlestick_data_map &candles_,
                                   std::list<std::shared_ptr<order_event>, std::allocator<std::shared_ptr<order_event>>>::iterator &it_)
{

}

void exchange::handle_market_event(const std::shared_ptr<event> &event_)
{
    auto &market_event_ = dynamic_cast<market_event &>(*event_);
    for (const auto &subscriber_ : m_market_event_subscribers) {
        subscriber_->notify_of_market_event(market_event_);
    }
}

void exchange::add_event(std::shared_ptr<event> &event_)
{
    m_event_queue.push(event_);
}

void exchange::subscribe_to_datastream(const std::shared_ptr<data_event_subscription> &subscription_)
{
    m_data_event_subscribers.push_back(subscription_);
}

}
