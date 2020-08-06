//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_MARKETEVENT_HPP
#define LUDERE_MARKETEVENT_HPP


#include <cstdint>

#include <ludere/Event.hpp>

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
    explicit MarketEvent(MarketEventType _mtype)
            : marketEventType(_mtype)
    {
        type = EventType::kMarketEvent;
    }

public:
    MarketEventType marketEventType;
};

}


#endif //LUDERE_MARKETEVENT_HPP
