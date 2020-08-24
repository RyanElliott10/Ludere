//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EVENT_HPP
#define LUDERE_EVENT_HPP


#include <ludere/order.hpp>

namespace lud {

enum class event_type : uint8_t
{
    ORDER_EVENT, // A request to fill an Order
    MARKET_EVENT, // A market-wide event. Market open/close, trading halt, etc.
};

class event
{
public:
    virtual ~event() = default;
    event_type m_type;
};

inline std::ostream &operator<<(std::ostream &strm_, const event_type &type_)
{
    switch (type_) {
    case event_type::ORDER_EVENT:
        return strm_ << "ORDER_EVENT";
    case event_type::MARKET_EVENT:
        return strm_ << "MARKET_EVENT";
    }
}

}


#endif //LUDERE_EVENT_HPP
