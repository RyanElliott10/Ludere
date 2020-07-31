//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_STRATEGY_HPP
#define LUDERE_STRATEGY_HPP


#include <ludere_private/MarketEventSubscriber.hpp>

namespace lud {

class Strategy : virtual public MarketEventSubscriber
{
public:
    Strategy();

    void beingTrading();
    virtual void notifyOfMarketEvent(MarketEvent &event);
};

}


#endif //LUDERE_STRATEGY_HPP
