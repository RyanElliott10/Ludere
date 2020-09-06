//
// Created by Ryan Elliott on 9/5/20.
//

#include <ludere/brokerage_fees.hpp>

namespace lud {

brokerage_fees::brokerage_fees(float order_price_)
        : m_order_price(order_price_), m_per_security_price(0)
{}

brokerage_fees::brokerage_fees(float order_price_, float per_security_price_)
        : m_order_price(order_price_), m_per_security_price(per_security_price_)
{}

float brokerage_fees::fees_for_order(const std::shared_ptr<order> &order_) const
{
    return m_order_price + (m_per_security_price * order_->m_num_shares);
}

float brokerage_fees::fees_for_order(const std::shared_ptr<filled_order> &filled_order_) const
{
    return m_order_price + (m_per_security_price * filled_order_->m_num_shares);
}

}