//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EVENT_HPP
#define LUDERE_EVENT_HPP


namespace lud {

enum class EventType : uint8_t
{
    kMarketEvent = 0,
    kFillEvent
};

class Event
{
public:
    EventType type;
};

}


#endif //LUDERE_EVENT_HPP
