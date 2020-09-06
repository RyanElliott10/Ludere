//
// Created by Ryan Elliott on 9/6/20.
//

#ifndef SUCCESSOR_LIMIT_ORDER_HPP
#define SUCCESSOR_LIMIT_ORDER_HPP


#include <ludere/order.hpp>

namespace lud {

struct limit_order : public order
{
    limit_order(std::string security_, uint32_t num_shares_, enums::order::signals order_signal_,
                enums::order::position_types position_type_, float limit_price_, order_lifetime order_lifetime_,
                strategy_callback_def callback_);

    [[nodiscard]] __attribute__((always_inline))
    float max_order_cost();

    static std::unique_ptr<filled_order> generate_filled_order_failure(const std::shared_ptr<limit_order> &order_,
                                                                       enums::order::fill_statuses order_status_,
                                                                       time_t timestamp_);

    float m_limit_price;
};

inline std::ostream &operator<<(std::ostream &strm_, const limit_order &order_);

}


#endif //SUCCESSOR_LIMIT_ORDER_HPP
