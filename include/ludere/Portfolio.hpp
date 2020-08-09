
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_PORTFOLIO_HPP
#define LUDERE_PORTFOLIO_HPP


#include <ludere/Exchange.hpp>
#include <ludere/FillEvent.hpp>
#include <ludere/FilledOrder.hpp>
#include <ludere/Order.hpp>
#include <ludere/Position.hpp>

namespace lud {

class Portfolio
{
public:
    Portfolio(Exchange &exchange, float cash);

    void handleFillEventConcluded(FilledOrder &filledOrder);
    void placeOrder(std::shared_ptr<Order> &order);
    [[nodiscard]] bool verifyCapital(float totalCost) const;

private:
    Exchange &m_exchange;
    float m_portfolioValue;
    float m_liquidCash;
    int m_numTrades;
    std::vector<Position> m_positions;
};

}


#endif //LUDERE_PORTFOLIO_HPP
