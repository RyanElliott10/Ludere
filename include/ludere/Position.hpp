//
// Created by Ryan Elliott on 8/8/20.
//

#ifndef SUCCESSOR_POSITION_HPP
#define SUCCESSOR_POSITION_HPP


#include <ludere/FilledOrder.hpp>
#include <ludere/Order.hpp>
#include <utility>

namespace lud {

struct Position
{
    explicit Position(std::shared_ptr<FilledOrder> &filledOrder_)
            : filledOrder(filledOrder_)
    {}

    std::shared_ptr<Order> order;
    std::shared_ptr<FilledOrder> filledOrder;
};

struct Holding
{
    explicit Holding(std::string security_)
            : security(std::move(security_)), numShares(0)
    {}

    std::vector<Position> m_positions;
    std::string security;
    float numShares;
};

}

namespace std {

template<>
struct hash<lud::Holding>
{
    inline size_t operator()(const lud::Holding &holding) const
    {
        return hash<std::string>()(holding.security);
    }
};

}


#endif //SUCCESSOR_POSITION_HPP
