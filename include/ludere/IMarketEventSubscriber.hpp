//
// Created by Ryan Elliott on 7/31/20.
//

#ifndef LUDERE_MARKETEVENTSUBSCRIBER_HPP
#define LUDERE_MARKETEVENTSUBSCRIBER_HPP


#include <ludere/MarketEvent.hpp>

namespace lud {

struct IMarketEventSubscriber
{
    virtual ~IMarketEventSubscriber() = default;

    virtual void notifyOfMarketEvent(MarketEvent &marketEvent) = 0;
};

}


#endif //LUDERE_MARKETEVENTSUBSCRIBER_HPP
