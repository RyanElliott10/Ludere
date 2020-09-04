//
// Created by Ryan Elliott on 8/24/20.
//

#ifndef SUCCESSOR_BROKERAGE_FEES_HPP
#define SUCCESSOR_BROKERAGE_FEES_HPP


#include <ludere/filled_order.hpp>
#include <ludere/order.hpp>

namespace lud {

struct brokerage_fees
{
  brokerage_fees() = default;

  explicit brokerage_fees(float order_price_)
          : m_order_price(order_price_), m_per_security_price(0)
  {}

  brokerage_fees(float order_price_, float per_security_price_)
          : m_order_price(order_price_), m_per_security_price(per_security_price_)
  {}

  float fees_for_order(const std::shared_ptr<order> &order_) const
  {
    return m_order_price + (m_per_security_price * order_->m_num_shares);
  }

  float fees_for_order(const std::shared_ptr<filled_order> &filled_order_) const
  {
    return m_order_price + (m_per_security_price * filled_order_->m_num_shares);
  }

  float m_order_price;
  float m_per_security_price;
};

}


#endif //SUCCESSOR_BROKERAGE_FEES_HPP
