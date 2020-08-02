//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_STRATEGY_HPP
#define LUDERE_STRATEGY_HPP


#include <ludere/IMarketEventSubscriber.hpp>
#include <ludere/CandlestickData.hpp>

namespace lud {

class AbstractStrategy : IMarketEventSubscriber
{
public:
    virtual void beginTrading()
    {
        m_isTrading = true;
    };

    virtual void notifyOfMarketEvent(MarketEvent &event) = 0;

    virtual void handleMarketData(CandlestickData &data) = 0;

private:
    bool m_isTrading = false;
};

}


#endif //LUDERE_STRATEGY_HPP
