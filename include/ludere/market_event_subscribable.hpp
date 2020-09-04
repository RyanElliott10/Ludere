//
// Created by Ryan Elliott on 7/31/20.
//

#ifndef LUDERE_MARKETEVENTSUBSCRIBER_HPP
#define LUDERE_MARKETEVENTSUBSCRIBER_HPP


#include <ludere/market_event.hpp>

namespace lud {

struct market_event_subscribable
{
  virtual ~market_event_subscribable() = default;

  virtual void notify_of_market_event(market_event &market_event_) = 0;
};

}


#endif //LUDERE_MARKETEVENTSUBSCRIBER_HPP
