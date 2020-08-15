
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_PORTFOLIO_HPP
#define LUDERE_PORTFOLIO_HPP


#include <unordered_map>

#include <boost/function.hpp>

#include <ludere/Exchange.hpp>
#include <ludere/FillEvent.hpp>
#include <ludere/FilledOrder.hpp>
#include <ludere/Order.hpp>
#include <ludere/Position.hpp>
#include <ludere/UUID.hpp>

namespace lud {

class Portfolio
{
public:
    Portfolio(Exchange &exchange, float cash);

    void handleFillEventConcluded(std::shared_ptr<FilledOrder> &filledOrder);
    void placeOrder(std::shared_ptr<Order> order);
    [[nodiscard]] bool verifyCapital(float totalCost) const;

    void updateHistoric(const CandlestickData &data);

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
