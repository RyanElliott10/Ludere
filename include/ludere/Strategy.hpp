//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_STRATEGY_HPP
#define LUDERE_STRATEGY_HPP


#include <ludere/MarketEventSubscriber.hpp>

namespace lud {

class Strategy : virtual public MarketEventSubscriber
{
public:
    Strategy();

    void beingTrading();
    void notifyOfMarketEvent(MarketEvent &event) override = 0;
};

}


#endif //LUDERE_STRATEGY_HPP
