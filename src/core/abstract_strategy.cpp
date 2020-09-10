//
// Created by Ryan Elliott on 9/5/20.
//

#include <ludere/abstract_strategy.hpp>
#include <ludere/candlestick_data.hpp>
#include <ludere/order.hpp>
#include <ludere/portfolio.hpp>

#include <utility>

namespace lud {

abstract_strategy::abstract_strategy(std::shared_ptr<portfolio> portfolio_)
        : m_portfolio(std::move(portfolio_))
{}

abstract_strategy::abstract_strategy(std::shared_ptr<portfolio> portfolio_, bool current_data_)
        : m_portfolio(std::move(portfolio_)), m_keep_data_current(current_data_)
{}

void abstract_strategy::trade()
{
    m_is_trading = true;
}

void abstract_strategy::place_limit_order(const std::string &security_, uint32_t num_shares_,
                                          enums::order::signals signal_, enums::order::position_types position_type_,
                                          order_lifetime lifetime_, float limit_price_)
{
    std::shared_ptr<order> order_ = std::make_shared<limit_order>(security_, num_shares_, signal_, position_type_,
                                                                  limit_price_, lifetime_, [this](auto &&PH1) {
                handle_concluded_order(PH1);
            });
    m_portfolio->place_order(order_);
}

}
