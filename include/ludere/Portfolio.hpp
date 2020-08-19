
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_PORTFOLIO_HPP
#define LUDERE_PORTFOLIO_HPP


#include <unordered_map>

#include <boost/function.hpp>

#include <ludere/Exchange.hpp>
#include <ludere/OrderEvent.hpp>
#include <ludere/FilledOrder.hpp>
#include <ludere/IDataEventSubscriber.hpp>
#include <ludere/Order.hpp>
#include <ludere/Position.hpp>
#include <ludere/UUID.hpp>

namespace lud {

class Portfolio : public IDataEventSubscriber
{
public:
    Portfolio(Exchange &exchange, float cash);

    void handleOrderEventConcluded(std::shared_ptr<FilledOrder> &filledOrder);
    void placeOrder(std::shared_ptr<Order> order);

    /// Verify a Portfolio has enough capital to perform a trade.
    [[nodiscard]] bool verifyCapital(float totalCost) const;

    void handleMarketData(const std::unordered_map<std::string, lud::CandlestickData> &data);

    /// Update the Portfolio's balance against the current positions and current market data
    void updateHistoric(const CandlestickDataMap &data);

    [[nodiscard]] inline float netValue() const { return m_portfolioValue + m_liquidCash; }

private:
    Exchange &m_exchange;
    float m_portfolioValue;
    float m_liquidCash;
    int m_numTrades;
    std::vector<Position> m_positions;
    std::unordered_map<UUIDHash, StrategyCallbackDef> m_orderCallbacks;
    std::unordered_map<UUIDHash, std::shared_ptr<Order>> m_allOrders; // All orders, even cancelled and failures
    std::unordered_map<UUIDHash, std::shared_ptr<FilledOrder>> m_allFilledOrders;
};

}


#endif //LUDERE_PORTFOLIO_HPP
