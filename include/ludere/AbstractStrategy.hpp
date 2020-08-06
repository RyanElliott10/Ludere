//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_STRATEGY_HPP
#define LUDERE_STRATEGY_HPP


#include <ludere/CandlestickData.hpp>
#include <ludere/DataRequestEvent.hpp>
#include <ludere/IMarketEventSubscriber.hpp>
#include <ludere/Portfolio.hpp>

namespace lud {

class AbstractStrategy : IMarketEventSubscriber
{
public:
    AbstractStrategy(Portfolio &portfolio)
            : m_portfolio(portfolio)
    {}

    AbstractStrategy(Portfolio &portfolio, bool currentData)
            : m_portfolio(portfolio), m_keepDataCurrent(currentData)
    {}

    virtual void trade()
    {
        m_isTrading = true;

        if (m_keepDataCurrent) {
            requestCandlestickData();
        }
    };

    virtual void notifyOfMarketEvent(lud::MarketEvent &event) = 0;

    virtual void handleMarketData(lud::CandlestickData &data)
    {
        if (m_keepDataCurrent) {
            requestCandlestickData();
        }
    }

    virtual void requestCandlestickData()
    {
        std::unique_ptr<Event> event = std::make_unique<DataRequestEvent>(
                std::bind(&AbstractStrategy::handleMarketData, this, std::placeholders::_1));
        m_portfolio.m_exchange.addEvent(std::move(event));
    }

protected:
    bool m_isTrading = false;
    bool m_keepDataCurrent = false;
    Portfolio &m_portfolio;
};

}


#endif //LUDERE_STRATEGY_HPP
