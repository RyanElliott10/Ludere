//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_FILLEVENT_HPP
#define LUDERE_FILLEVENT_HPP


#include <boost/function.hpp>

#include <ludere/Event.hpp>
#include <ludere/FilledOrder.hpp>
#include <ludere/Order.hpp>

namespace lud {

class FillEvent : public Event
{
public:
    explicit FillEvent(std::shared_ptr<Order> _order)
            : order(_order)
    {
        type = EventType::kFillEvent;
    }

public:
    std::shared_ptr<Order> order;
    boost::function<void(FilledOrder &)> callback;
    boost::function<bool(float)> verifyPortfolioFunds;
};

}


#endif //LUDERE_FILLEVENT_HPP
