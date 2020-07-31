//
// Created by Ryan Elliott on 7/31/20.
//

#ifndef LUDERE_MARKETEVENTSUBSCRIBER_HPP
#define LUDERE_MARKETEVENTSUBSCRIBER_HPP


#include <ludere/Events.hpp>

namespace lud {

// A pure interface for classes to implement to be notified of MarketEvents
struct MarketEventSubscriber
{
    virtual ~MarketEventSubscriber() = default;
    virtual void notifyOfMarketEvent(MarketEvent &marketEvent) = 0;
};

}


#endif //LUDERE_MARKETEVENTSUBSCRIBER_HPP
