//
// Created by Ryan Elliott on 9/6/20.
//

#include "ludere/market_order.hpp"


namespace lud {

market_order::market_order(std::string security_, uint32_t num_share_, enums::order::signals order_signal_,
                           enums::order::position_types position_type_, float market_price_,
                           order_lifetime order_lifetime_,
                           strategy_callback_def callback_)
        : order(std::move(security_), num_share_, order_signal_, enums::order::types::MARKET, position_type_,
                order_lifetime_, std::move(callback_)), m_market_price(market_price_)
{}

std::unique_ptr<filled_order> market_order::generate_filled_order_failure(const std::shared_ptr<market_order> &order_,
                                                                          const enums::order::fill_statuses order_status_,
                                                                          time_t timestamp_)
{
    return std::move(std::make_unique<filled_order>(order_->m_security, order_->m_num_shares, order_->m_market_price,
                                                    order_->m_order_signal, order_status_, timestamp_, order_->m_uuid));
}

inline std::ostream &operator<<(std::ostream &strm_, const market_order &order_)
{
    return strm_ << order_.m_security << " " << order_.m_num_shares << " @ $" << order_.m_market_price;
}

}
