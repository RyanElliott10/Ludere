//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EVENT_HPP
#define LUDERE_EVENT_HPP


#include <ludere/Order.hpp>

namespace lud {

enum class EventType : uint8_t
{
    kMarketEvent = 0, // A market-wide event. Market open/close, trading halt, etc.
    kFillEvent // A request to fill an Order
};

class Event
{
public:
    EventType type;
};

}


#endif //LUDERE_EVENT_HPP
