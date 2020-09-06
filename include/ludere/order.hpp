//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_ORDER_HPP
#define LUDERE_ORDER_HPP


#include <string>
#include <iostream>

#include <boost/function.hpp>

#include <ludere/filled_order.hpp>
#include <ludere/order_enums.hpp>
#include <ludere/uuid.hpp>

// TODO: Reconsider the raw data storage of the Order class (and it's subclasses). Consider breaking data into smaller
//      objects. Further reading: https://softwareengineering.stackexchange.com/questions/305313/avoiding-constructors-with-many-arguments
namespace lud {

using strategy_callback_def = boost::function<void(std::shared_ptr<filled_order>)>;

// TODO: Create cpp file for order_lifetime
/// duration an order should remain active
struct order_lifetime
{
    explicit order_lifetime(enums::order::lifetime_durations type_, time_t baseTime_)
            : m_duration(int(type_)), m_base_time(baseTime_)
    {}

    [[maybe_unused]] explicit order_lifetime(time_t custom_duration_, time_t baseTime_)
            : m_duration(custom_duration_), m_base_time(baseTime_)
    {}

    /// duration, in seconds
    time_t m_duration;
    time_t m_base_time;
};

struct order
{
    order(std::string security_, uint32_t num_shares_, enums::order::signals order_signal_,
          enums::order::types order_type_, enums::order::position_types position_type_, order_lifetime order_lifetime_,
          strategy_callback_def callback_);

    [[nodiscard]] __attribute__((always_inline))
    bool is_expired(time_t current_time) const;

    virtual ~order() = default;

    // OrderAmounts
    std::string m_security;
    uint32_t m_num_shares;

    // Can be grouped into some class, OrderMetaData
    enums::order::signals m_order_signal;
    enums::order::position_types m_position_type;
    enums::order::types m_order_type;
    order_lifetime m_order_lifetime;

    // OrderHandling
    strategy_callback_def m_strategy_callback;

    uuid m_uuid;
};

inline std::ostream &operator<<(std::ostream &strm, const order &order_);

}


#endif //LUDERE_ORDER_HPP
