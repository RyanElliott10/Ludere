//
// Created by Ryan Elliott on 9/6/20.
//

#include <ludere/order.hpp>

namespace lud {

order::order(std::string security_, uint32_t num_shares_, enums::order::signals order_signal_,
             enums::order::types order_type_, enums::order::position_types position_type_, order_lifetime order_lifetime_,
             strategy_callback_def callback_)
        : m_security(std::move(security_)), m_num_shares(num_shares_), m_order_signal(order_signal_),
          m_order_type(order_type_), m_position_type(position_type_), m_order_lifetime(order_lifetime_),
          m_strategy_callback(std::move(callback_)), m_uuid(uuid())
{}

bool order::is_expired(time_t current_time) const
{
    return (m_order_lifetime.m_base_time + m_order_lifetime.m_duration) < current_time;
}

inline std::ostream &operator<<(std::ostream &strm, const order &order_)
{
    return strm << order_.m_security;
}

}