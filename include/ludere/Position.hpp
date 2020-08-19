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

}


#endif //SUCCESSOR_POSITION_HPP
