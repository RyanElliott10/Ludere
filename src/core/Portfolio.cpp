//
// Created by Ryan Elliott on 7/30/20.
//

#include <ludere/Portfolio.hpp>

namespace lud {

Portfolio::Portfolio(Exchange &exchange, float cash)
        : m_exchange(exchange), m_liquidCash(cash), m_portfolioValue(cash), m_numTrades(0)
{}

void Portfolio::handleFillEventConcluded(FilledOrder &filledOrder)
{
    if (filledOrder.orderStatus == FilledOrder::FilledOrderStatus::kSuccess) {
        std::cout << "Total cost: " << filledOrder.totalOrderCost() << " for " << filledOrder.security << std::endl;
        m_liquidCash -= filledOrder.totalOrderCost();
        m_numTrades++;
        m_positions.emplace_back();

        // Usage of map to call strategy's callback
    } else {
        // uh oh
    }
}

void Portfolio::placeOrder(std::shared_ptr<Order> &order)
{
    std::shared_ptr<Event> event = std::make_shared<FillEvent>(order);
    std::dynamic_pointer_cast<FillEvent>(event)->callback = [this](auto &&PH1) { handleFillEventConcluded(PH1); };
    std::dynamic_pointer_cast<FillEvent>(event)->verifyPortfolioFunds = [this](float totalCost) {
        return verifyCapital(totalCost);
    };

    m_orderCallbacks.insert(std::pair<UUIDHash, StrategyCallbackDef>(order->uuid.hash(), order->strategyCallback));
    m_exchange.addEvent(event);
}

bool Portfolio::verifyCapital(float totalCost) const
{
    return m_liquidCash >= totalCost;
}

}
