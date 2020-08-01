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
    Strategy() {
        m_isTrading = false;
    }

    virtual void beginTrading() {
        m_isTrading = true;
    };
    virtual void notifyOfMarketEvent(MarketEvent &event) = 0;

private:
    bool m_isTrading;
};

}


#endif //LUDERE_STRATEGY_HPP
