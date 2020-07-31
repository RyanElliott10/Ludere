//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_MARKETEVENT_HPP
#define LUDERE_MARKETEVENT_HPP


#include <stdint.h>

#include <ludere/Event.hpp>
#include <ludere/Order.hpp>

namespace lud {

enum class MarketEventType : uint8_t
{
    kMarketOpen = 0,
    kMarketClose,
    kMarketHalt
};

class MarketEvent : public Event
{
public:
    MarketEventType marketEventType;
};

}


#endif //LUDERE_MARKETEVENT_HPP
