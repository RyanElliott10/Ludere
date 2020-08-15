//
// Created by Ryan Elliott on 7/30/20.
//

#include <ludere/Portfolio.hpp>

namespace lud {

Portfolio::Portfolio(Exchange &exchange, float cash)
        : m_exchange(exchange), m_liquidCash(cash), m_portfolioValue(cash), m_numTrades(0)
{}

void Portfolio::handleFillEventConcluded(std::shared_ptr<FilledOrder> &filledOrder)
{
    if (filledOrder->orderStatus == FilledOrder::FilledOrderStatus::kSuccess) {
        std::cout << "Total cost: " << filledOrder->totalOrderCost() << " for " << filledOrder->security << std::endl;
        m_positions.emplace_back(filledOrder);
        m_liquidCash -= filledOrder->totalOrderCost();
        m_numTrades++;
    }

    m_orderCallbacks.at(filledOrder->uuid.hash())(filledOrder);
    m_allFilledOrders.emplace(filledOrder->uuid.hash(), filledOrder);
}

void Portfolio::placeOrder(std::shared_ptr<Order> order)
{
    m_allOrders.emplace(order->uuid.hash(), order);
    std::shared_ptr<Event> event = std::make_shared<FillEvent>(order);
    std::dynamic_pointer_cast<FillEvent>(event)->callback = [this](auto &&PH1) { handleFillEventConcluded(PH1); };
    std::dynamic_pointer_cast<FillEvent>(event)->verifyPortfolioFunds = [this](float totalCost) {
        return verifyCapital(totalCost);
    };

    m_orderCallbacks.emplace(order->uuid.hash(), order->strategyCallback);
    m_exchange.addEvent(event);
}

bool Portfolio::verifyCapital(float totalCost) const
{
    return m_liquidCash >= totalCost;
}

/**
 * Updates the portfolio's historic value against a CandlestickData instance.
 * TODO: Perhaps subscribe a portfolio to a datastream. Further, support requests for a specific subset of securities.
 *      If a Portfolio is only interested in Apple, TSLA, and SPY, it should only receive information about those
 *      securities. In other words, plan out the complete data streaming flow and capabilities
 * @param data
 */
void Portfolio::updateHistoric(const CandlestickData &data)
{
    for (auto position : m_positions) {

    }
}

}
