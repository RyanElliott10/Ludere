//
// Created by Ryan Elliott on 8/8/20.
//

#ifndef SUCCESSOR_FILLED_ORDER_HPP
#define SUCCESSOR_FILLED_ORDER_HPP


#include <chrono>
#include <cstdint>
#include <string>

#include <ludere/order_enums.hpp>
#include <ludere/uuid.hpp>

namespace lud {

struct filled_order
{
    filled_order(std::string security_, uint32_t num_shares_, float share_price_, enums::order::signals order_signal_,
                 enums::order::fill_statuses order_status_, time_t timestamp_, uuid uuid_);

    [[nodiscard]]  __attribute__((always_inline))
    float total_order_cost() const;

    std::string m_security;
    uint32_t m_num_shares;
    float m_share_price;
    enums::order::fill_statuses m_order_status;
    time_t m_timestamp;
    uuid m_uuid;
    enums::order::signals m_order_signal;
};

}


#endif //SUCCESSOR_FILLED_ORDER_HPP
