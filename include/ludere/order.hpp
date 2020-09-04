//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_ORDER_HPP
#define LUDERE_ORDER_HPP


#include <chrono>
#include <string>
#include <utility>
#include <iostream>

#include <boost/function.hpp>

#include <ludere/filled_order.hpp>
#include <ludere/order_enums.hpp>
#include <ludere/uuid.hpp>

// TODO: Reconsider the raw data storage of the Order class (and it's subclasses). Consider breaking data into smaller
//      objects. Further reading: https://softwareengineering.stackexchange.com/questions/305313/avoiding-constructors-with-many-arguments
namespace lud {

using strategy_callback_def = boost::function<void(std::shared_ptr<filled_order>)>;

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
        strategy_callback_def callback_)
          : m_security(std::move(security_)), m_num_shares(num_shares_), m_order_signal(order_signal_),
            m_order_type(order_type_), m_position_type(position_type_), m_order_lifetime(order_lifetime_),
            m_strategy_callback(std::move(callback_)), m_uuid(uuid())
  {}

  [[nodiscard]] inline bool is_expired(time_t current_time) const
  {
    return (m_order_lifetime.m_base_time + m_order_lifetime.m_duration) < current_time;
  }

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

struct limit_order : public order
{
  limit_order(std::string security_, uint32_t num_shares_, enums::order::signals order_signal_,
              enums::order::position_types position_type_, float limit_price_, order_lifetime order_lifetime_,
              strategy_callback_def callback_)
          : order(std::move(security_), num_shares_, order_signal_, enums::order::types::LIMIT, position_type_,
                  order_lifetime_, std::move(callback_)), m_limit_price(limit_price_)
  {}

  inline float max_order_cost()
  {
    return m_limit_price * m_num_shares;
  }

  static std::unique_ptr<filled_order> generate_filled_order_failure(const std::shared_ptr<limit_order> &order_,
                                                                     const enums::order::fill_statuses order_status_,
                                                                     time_t timestamp_)
  {
    return std::move(std::make_unique<filled_order>(order_->m_security, order_->m_num_shares, order_->m_limit_price,
                                                    order_->m_order_signal, order_status_, timestamp_,
                                                    order_->m_uuid));
  }

  float m_limit_price;
};

struct market_order : public order
{
  market_order(std::string security_, uint32_t num_share_, enums::order::signals order_signal_,
               enums::order::position_types position_type_, float market_price_, order_lifetime order_lifetime_,
               strategy_callback_def callback_)
          : order(std::move(security_), num_share_, order_signal_, enums::order::types::MARKET, position_type_,
                  order_lifetime_, std::move(callback_)), m_market_price(market_price_)
  {}

  static std::unique_ptr<filled_order> generate_filled_order_failure(const std::shared_ptr<market_order> &order_,
                                                                     const enums::order::fill_statuses order_status_,
                                                                     time_t timestamp_)
  {
    return std::move(
            std::make_unique<filled_order>(order_->m_security, order_->m_num_shares, order_->m_market_price,
                                           order_->m_order_signal, order_status_, timestamp_, order_->m_uuid));
  }

  float m_market_price;
};

inline std::ostream &operator<<(std::ostream &strm_, const limit_order &order_)
{
  return strm_ << order_.m_security << " " << order_.m_num_shares << " @ $" << order_.m_limit_price;
}

inline std::ostream &operator<<(std::ostream &strm, const order &order_)
{
  return strm << order_.m_security;
}

inline std::ostream &operator<<(std::ostream &strm_, const market_order &order_)
{
  return strm_ << order_.m_security << " " << order_.m_num_shares << " @ $" << order_.m_market_price;
}

}


#endif //LUDERE_ORDER_HPP
