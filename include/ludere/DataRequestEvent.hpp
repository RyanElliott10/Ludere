//
// Created by Ryan Elliott on 8/1/20.
//

#ifndef SUCCESSOR_DATAREQUESTEVENT_HPP
#define SUCCESSOR_DATAREQUESTEVENT_HPP


#include <ludere/Event.hpp>
#include <ludere/AbstractStrategy.hpp>

namespace lud {

class DataRequestEvent : public Event
{
public:
    explicit DataRequestEvent(AbstractStrategy &_strategy)
            : strategy(_strategy)
    {
        type = EventType::kDataRequestEvent;
    }

    AbstractStrategy &strategy;
};

}

#endif //SUCCESSOR_DATAREQUESTEVENT_HPP
