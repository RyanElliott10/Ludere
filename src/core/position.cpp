//
// Created by Ryan Elliott on 9/6/20.
//

#include <ludere/position.hpp>

namespace lud {

position::position(std::shared_ptr<order> &order_, std::shared_ptr<filled_order> &filledOrder_)
        : m_order(order_), m_filled_order(filledOrder_)
{}

holding::holding(std::string security_)
        : m_security(std::move(security_)), m_num_shares(0)
{}

}

size_t std::hash<lud::holding>::operator()(const lud::holding &holding_) const
{
    return hash<std::string>()(holding_.m_security);
}
