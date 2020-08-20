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
#include <utility>

namespace lud {

class AbstractStrategy : public IDataEventSubscriber, public IMarketEventSubscriber
{
public:
    explicit AbstractStrategy(std::shared_ptr<Portfolio> portfolio)
            : m_portfolio(std::move(portfolio))
    {}

    AbstractStrategy(std::shared_ptr<Portfolio> portfolio, bool currentData)
            : m_portfolio(std::move(portfolio)), m_keepDataCurrent(currentData)
    {}

    /// Called as soon as trading on an Exchange begins.
    virtual void trade()
    {
        m_isTrading = true;
    };

    /// Main connection to the Portfolio to create and place a LimitOrder.
    virtual void
    placeLimitOrder(std::string security, uint32_t numShares, Order::PositionType positionType, OrderLifetime lifetime, float limitPrice)
    {
        std::shared_ptr<Order> order = std::make_shared<LimitOrder>(security, numShares, positionType, limitPrice,
                                                                    lifetime,
                                                                    [this](auto &&PH1) { handleConcludedOrder(PH1); });
        m_portfolio->placeOrder(order);
    }

    /// Called right before trading on the Exchange begins. Can be used as an alternative method to subscribe to an
    /// Exchange's data stream.
    virtual void prepareToTrade() = 0;

    /// Handler for when an order has been processed and returned by an Exchange. Order fulfillment is not guaranteed.
    virtual void handleConcludedOrder(std::shared_ptr<FilledOrder> filledOrder) = 0;

protected:
    bool m_isTrading = false;
    bool m_keepDataCurrent = false;
    std::shared_ptr<Portfolio> m_portfolio;
};

}


#endif //LUDERE_STRATEGY_HPP
