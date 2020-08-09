//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EVENT_HPP
#define LUDERE_EVENT_HPP


#include <ludere/Order.hpp>

namespace lud {

enum class EventType : uint8_t
{
    kFillEvent, // A request to fill an Order
    kMarketEvent, // A market-wide event. Market open/close, trading halt, etc.
};

class Event
{
public:
    virtual ~Event() = default;
    EventType type;
};

inline std::ostream &operator<<(std::ostream &strm, const EventType &type)
{
    switch (type) {
    case EventType::kFillEvent:
        return strm << "kFillEvent";
    case EventType::kMarketEvent:
        return strm << "kMarketEvent";
    }
}

}


#endif //LUDERE_EVENT_HPP
