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
#include <utility>

namespace lud {

class abstract_strategy : public data_event_subscribable, public market_event_subscribable
{
public:
    explicit abstract_strategy(std::shared_ptr<portfolio> portfolio_)
            : m_portfolio(std::move(portfolio_))
    {}

    abstract_strategy(std::shared_ptr<portfolio> portfolio_, bool current_data_)
            : m_portfolio(std::move(portfolio_)), m_keep_data_current(current_data_)
    {}

    /// Called as soon as trading on an exchange begins.
    virtual void trade()
    {
        m_is_trading = true;
    };

    /// Main connection to the Portfolio to create and place a LimitOrder.
    virtual void place_limit_order(const std::string &security_, uint32_t num_shares_, enums::order::signals signal_,
                                   enums::order::position_types position_type_, order_lifetime lifetime_,
                                   float limit_price_)
    {
        std::shared_ptr<order> order_ = std::make_shared<limit_order>(security_, num_shares_, signal_, position_type_,
                                                                      limit_price_, lifetime_, [this](auto &&PH1) {
                    handle_concluded_order(PH1);
                });
        m_portfolio->place_order(order_);
    }

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
