//
// Created by Ryan Elliott on 9/6/20.
//

#ifndef SUCCESSOR_MARKET_ORDER_HPP
#define SUCCESSOR_MARKET_ORDER_HPP


#include <ludere/order.hpp>

namespace lud {

struct market_order : public order
{

    market_order(std::string security_, uint32_t num_share_, enums::order::signals order_signal_,
                 enums::order::position_types position_type_, float market_price_, order_lifetime order_lifetime_,
                 strategy_callback_def callback_);

    static std::unique_ptr<filled_order> generate_filled_order_failure(const std::shared_ptr<market_order> &order_,
                                                                       enums::order::fill_statuses order_status_,
                                                                       time_t timestamp_);


    float m_market_price;
};

inline std::ostream &operator<<(std::ostream &strm_, const market_order &order_);

}


#endif //SUCCESSOR_MARKET_ORDER_HPP
