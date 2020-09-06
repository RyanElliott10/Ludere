//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_STRATEGY_HPP
#define LUDERE_STRATEGY_HPP


#include <ludere/candlestick_data.hpp>
#include <ludere/data_event_subscribable.hpp>
#include <ludere/market_event_subscribable.hpp>
#include <ludere/order.hpp>
#include <ludere/portfolio.hpp>

namespace lud {

class abstract_strategy : public data_event_subscribable, public market_event_subscribable
{
public:
    explicit abstract_strategy(std::shared_ptr<portfolio> portfolio_);
    abstract_strategy(std::shared_ptr<portfolio> portfolio_, bool current_data_);

    /// Called as soon as trading on an exchange begins.
    virtual void trade();

    /// Main connection to the Portfolio to create and place a LimitOrder.
    virtual void place_limit_order(const std::string &security_, uint32_t num_shares_, enums::order::signals signal_,
                                   enums::order::position_types position_type_, order_lifetime lifetime_,
                                   float limit_price_);

    /// Called right before trading on the exchange begins. Can be used as an alternative method to subscribe to an
    /// exchange's data stream.
    virtual void prepare_to_trade() = 0;

    /// Handler for when an order has been processed and returned by an exchange. Order fulfillment is not guaranteed.
    virtual void handle_concluded_order(std::shared_ptr<filled_order> filled_order_) = 0;

protected:
    bool m_is_trading = false;
    bool m_keep_data_current = false;
    std::shared_ptr<portfolio> m_portfolio;
};

}


#endif //LUDERE_STRATEGY_HPP
