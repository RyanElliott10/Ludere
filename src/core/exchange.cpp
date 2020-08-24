//
// Created by Ryan Elliott on 7/30/20.
//

#include <utility>

#include <ludere/candlestick_data.hpp>
#include <ludere/exchange.hpp>
#include <ludere/filled_order.hpp>


namespace lud {

exchange::exchange(std::shared_ptr<data_streamable> data_stream_)
        : m_dataStream(std::move(data_stream_)), m_is_trading(false)
{}

exchange::exchange(std::shared_ptr<data_streamable> data_stream_, const bool inverted_data_stream_)
        : m_dataStream(std::move(data_stream_)), m_inverted_data_stream(inverted_data_stream_), m_is_trading(false)
{
    if (m_inverted_data_stream) {
        LUD_ERROR("Inverted datastream is not yet supported", nullptr);
        exit(-1);
    }
}

void exchange::trade()
{
    m_is_trading = true;

    candlestick_data_map candles_;
    while (!(candles_ = m_dataStream->poll_next_stream()).empty()) {
        stream_data(candles_);

        while (!m_event_queue.empty()) {
            std::shared_ptr<event> event_ = m_event_queue.front();
            switch (event_->m_type) {
            case event_type::ORDER_EVENT:
                handle_order_event(event_, candles_);
                break;
            case event_type::MARKET_EVENT:
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
        if ((*it_)->m_order->is_expired(candles_.at("AAPL").m_timestamp)) {
            const std::shared_ptr<limit_order> &order_ = std::dynamic_pointer_cast<limit_order>((*it_)->m_order);
            std::unique_ptr<filled_order> filled_order_ = std::make_unique<filled_order>(order_->m_security, order_->m_num_shares,
                                                                                         0,
                                                                                         filled_order::filled_order_statuses::EXPIRED,
                                                                                         candles_.at("AAPL").m_timestamp,
                                                                                         order_->m_uuid);
            (*it_)->m_callback(std::move(filled_order_));
            it_ = m_order_queue.erase(it_);
            continue;
        }
        order::order_types &type_ = (*it_)->m_order->m_order_type;
        if (type_ == order::order_types::LIMIT) {
            handle_limit_order(std::dynamic_pointer_cast<limit_order>((*it_)->m_order), candles_, it_);
        } else if (type_ == order::order_types::MARKET) {
            handle_market_order(std::dynamic_pointer_cast<market_order>((*it_)->m_order), candles_, it_);
        }
        it_++;
    }
}

// TODO: Implement slippage and more realistic market conditions. This is guaranteed to fill your order at the best
//      price and immediately (as long as the security's price is below the limit)
void exchange::handle_limit_order(const std::shared_ptr<limit_order> &order_, const candlestick_data_map &candles_,
                                  std::list<std::shared_ptr<order_event>>::iterator &it_)
{
    const float security_price_ = candles_.at((*it_)->m_order->m_security).m_close;
    if (order_->m_limit_price >= security_price_ && (*it_)->m_verify_portfolio_funds(order_->m_num_shares * security_price_)) {
        std::unique_ptr<filled_order> filled_order_ = std::make_unique<filled_order>(order_->m_security, order_->m_num_shares,
                                                                                     security_price_,
                                                                                     filled_order::filled_order_statuses::SUCCESS,
                                                                                     candles_.at((*it_)->m_order->m_security).m_timestamp,
                                                                                     order_->m_uuid);
        (*it_)->m_callback(std::move(filled_order_));
        it_ = m_order_queue.erase(it_);
    }
}

/**
 * Typical MarketOrder handling: the order is filled at the highest market price.
 * TODO: Place on the top of the order queue. Will require using a PriorityQueue rather than a list, or can add
 *      additional logic.
 * @param order_
 * @param candles_
 * @param it_
 */
void exchange::handle_market_order(const std::shared_ptr<market_order> &order_, const candlestick_data_map &candles_,
                                   std::list<std::shared_ptr<order_event>>::iterator &it_)
{
    const float security_price_ = candles_.at((*it_)->m_order->m_security).m_close;
    if ((*it_)->m_verify_portfolio_funds(order_->m_num_shares * security_price_)) {
        std::unique_ptr<filled_order> filled_order_ = std::make_unique<filled_order>(order_->m_security, order_->m_num_shares,
                                                                                     security_price_,
                                                                                     filled_order::filled_order_statuses::SUCCESS,
                                                                                     candles_.at((*it_)->m_order->m_security).m_timestamp,
                                                                                     order_->m_uuid);
        (*it_)->m_callback(std::move(filled_order_));
        it_ = m_order_queue.erase(it_);
    }
}

void exchange::handle_market_event(const std::shared_ptr<event> &event_)
{
    auto &market_event_ = dynamic_cast<market_event &>(*event_);
    for (const auto &subscriber_ : m_market_event_subscribers) {
        subscriber_->notify_of_market_event(market_event_);
    }
}

void exchange::addEvent(std::shared_ptr<event> &event_)
{
    m_event_queue.push(event_);
}

void exchange::subscribe_to_data_stream(const std::shared_ptr<data_event_subscription> &subscription_)
{
    m_data_event_subscribers.push_back(subscription_);
}

}
