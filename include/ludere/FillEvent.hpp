//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_FILLEVENT_HPP
#define LUDERE_FILLEVENT_HPP


#include <ludere/Event.hpp>
#include <ludere/Order.hpp>

namespace lud {

class FillEvent : public Event
{
public:
    Order &order;
};

}


#endif //LUDERE_FILLEVENT_HPP
