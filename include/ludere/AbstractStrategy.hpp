//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_STRATEGY_HPP
#define LUDERE_STRATEGY_HPP


#include <ludere/CandlestickData.hpp>
#include <ludere/IDataEventSubscriber.hpp>
#include <ludere/IMarketEventSubscriber.hpp>
#include <ludere/Order.hpp>
#include <ludere/Portfolio.hpp>

namespace lud {

class AbstractStrategy : public IMarketEventSubscriber, public IDataEventSubscriber
{
public:
    explicit AbstractStrategy(Portfolio &portfolio)
            : m_portfolio(portfolio)
    {}

    AbstractStrategy(Portfolio &portfolio, bool currentData)
            : m_portfolio(portfolio), m_keepDataCurrent(currentData)
    {}

    virtual void trade()
    {
        m_isTrading = true;
    };

    virtual void prepareToTrade() = 0;

    virtual void
    placeLimitOrder(std::string &security, uint32_t numShares, Order::PositionType positionType, float limitPrice)
    {
        std::shared_ptr<Order> order = std::make_shared<LimitOrder>(security, numShares, positionType, limitPrice);
        m_portfolio.placeOrder(order);
    }

protected:
    bool m_isTrading = false;
    bool m_keepDataCurrent = false;
    Portfolio &m_portfolio;
};

}


#endif //LUDERE_STRATEGY_HPP
