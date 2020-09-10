//
// Created by Ryan Elliott on 9/6/20.
//

#include <ludere/filled_order.hpp>

namespace lud {

filled_order::filled_order(std::string security_, uint32_t num_shares_, float share_price_,
                           enums::order::signals order_signal_,
                           enums::order::fill_statuses order_status_, time_t timestamp_, uuid uuid_)
        : m_security(std::move(security_)), m_num_shares(num_shares_), m_share_price(share_price_),
          m_order_signal(order_signal_), m_order_status(order_status_), m_timestamp(timestamp_), m_uuid(uuid_)
{}

float filled_order::total_order_cost() const
{
    return m_num_shares * m_share_price;
}

}
