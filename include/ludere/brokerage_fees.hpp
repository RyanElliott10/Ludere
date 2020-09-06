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
    explicit brokerage_fees(float order_price_);
    brokerage_fees(float order_price_, float per_security_price_);

    [[nodiscard]] float fees_for_order(const std::shared_ptr<order> &order_) const;
    [[nodiscard]] float fees_for_order(const std::shared_ptr<filled_order> &filled_order_) const;

    float m_order_price;
    float m_per_security_price;
};

}


#endif //SUCCESSOR_BROKERAGE_FEES_HPP
