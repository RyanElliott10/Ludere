//
// Created by Ryan Elliott on 8/1/20.
//

#ifndef SUCCESSOR_DATAREQUESTEVENT_HPP
#define SUCCESSOR_DATAREQUESTEVENT_HPP


#include <ludere/CandlestickData.hpp>
#include <ludere/Event.hpp>
#include <utility>

namespace lud {

/**
 * TODO: There is a circular dependency issue as this stands. AbstractStrategy requires DataRequestEvent, and
 *  DataRequestEvent requires AbstractStrategy. Simply remove the reference to a strategy in favor of a function
 *  (std::function)
 */
class DataRequestEvent : public Event
{
public:
    explicit DataRequestEvent(std::function<void(CandlestickData & )> _callback)
            : callback(std::move(_callback))
    {
        type = EventType::kDataRequestEvent;
    }

    std::function<void(CandlestickData & )> callback;
};

}

#endif //SUCCESSOR_DATAREQUESTEVENT_HPP
