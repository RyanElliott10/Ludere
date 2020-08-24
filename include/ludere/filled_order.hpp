//
// Created by Ryan Elliott on 8/8/20.
//

#ifndef SUCCESSOR_FILLED_ORDER_HPP
#define SUCCESSOR_FILLED_ORDER_HPP


#include <chrono>
#include <cstdint>
#include <string>

#include <ludere/uuid.hpp>

namespace lud {

struct filled_order
{
    enum class filled_order_statuses
    {
        SUCCESS,
        INSUFFICIENT_FUNDS,
        EXPIRED
    };

    filled_order(std::string security_, uint32_t numShares_, float sharePrice_, filled_order_statuses orderStatus_,
                 time_t timestamp_, uuid uuid_)
            : m_security(std::move(security_)), m_num_shares(numShares_), m_share_price(sharePrice_),
              m_order_status(orderStatus_), m_timestamp(timestamp_), m_uuid(uuid_)
    {}

    [[nodiscard]] float total_order_cost() const
    {
        return m_num_shares * m_share_price;
    }

    static std::unique_ptr<filled_order>
    generate_failed_order_failure(filled_order_statuses order_status_, time_t timestamp_, uuid uuid_)
    {
        return std::move(std::make_unique<filled_order>(nullptr, 0, 0, order_status_, timestamp_, uuid_));
    }

    std::string m_security;
    uint32_t m_num_shares;
    float m_share_price;
    filled_order_statuses m_order_status;
    time_t m_timestamp;
    uuid m_uuid;
};

}


#endif //SUCCESSOR_FILLED_ORDER_HPP
