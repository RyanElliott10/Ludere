//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_MARKETEVENT_HPP
#define LUDERE_MARKETEVENT_HPP


#include <cstdint>

#include <ludere/event.hpp>

namespace lud {

enum class market_event_type : uint8_t
{
  MARKET_OPEN = 0,
  MARKET_CLOSE,
  MARKET_HALT
};

class market_event : public event
{
public:
  explicit market_event(market_event_type mtype_)
          : m_market_event_type(mtype_)
  {
    m_type = event_type::MARKET;
  }

public:
  market_event_type m_market_event_type;
};

}


#endif //LUDERE_MARKETEVENT_HPP
