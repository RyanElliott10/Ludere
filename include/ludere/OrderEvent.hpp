//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_ORDEREVENT_HPP
#define LUDERE_ORDEREVENT_HPP


#include <boost/function.hpp>

#include <ludere/Event.hpp>
#include <ludere/FilledOrder.hpp>
#include <ludere/Order.hpp>

namespace lud {

class OrderEvent : public Event
{
public:
    explicit OrderEvent(std::shared_ptr<Order> _order)
            : order(std::move(_order))
    {
        type = EventType::kOrderEvent;
    }

public:
    std::shared_ptr<Order> order;
    boost::function<void(std::shared_ptr<FilledOrder>)> callback;
    boost::function<bool(float)> verifyPortfolioFunds;
};

}


#endif //LUDERE_ORDEREVENT_HPP
