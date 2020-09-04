//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EVENT_HPP
#define LUDERE_EVENT_HPP


#include <ludere/order.hpp>

namespace lud {

enum class event_type : uint8_t
{
  ORDER, // A request to fill an Order
  MARKET, // A market-wide event. Market open/close, trading halt, etc.
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
  case event_type::ORDER:
    return strm_ << "ORDER";
  case event_type::MARKET:
    return strm_ << "MARKET";
  }
}

}


#endif //LUDERE_EVENT_HPP
