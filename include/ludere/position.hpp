//
// Created by Ryan Elliott on 8/8/20.
//

#ifndef SUCCESSOR_POSITION_HPP
#define SUCCESSOR_POSITION_HPP


#include <ludere/filled_order.hpp>
#include <ludere/order.hpp>
#include <utility>

namespace lud {

struct position
{
    position() = delete;

    explicit position(std::shared_ptr<order> &order_, std::shared_ptr<filled_order> &filledOrder_);

    const std::shared_ptr<order> m_order;
    const std::shared_ptr<filled_order> m_filled_order;
};

struct holding
{
    holding() = delete;

    explicit holding(std::string security_);

    std::vector<position> m_positions;
    std::string m_security;
    int m_num_shares;
};

}

namespace std {

template<>
struct hash<lud::holding>
{

    __attribute__((always_inline))
    size_t operator()(const lud::holding &holding_) const;

};

}


#endif //SUCCESSOR_POSITION_HPP
