//
// Created by Ryan Elliott on 7/30/20.
//

#include <ludere/Portfolio.hpp>

namespace lud {

Portfolio::Portfolio(Exchange &exchange, const float cash)
        : m_exchange(exchange), m_startingCapital(cash), m_liquidCash(cash), m_portfolioValue(cash), m_numTrades(0)
{}

void Portfolio::handleOrderEventConcluded(std::shared_ptr<FilledOrder> &filledOrder)
{
    if (filledOrder->orderStatus == FilledOrder::FilledOrderStatus::kSuccess) {
        LUD_DEBUG("Total order cost: $%.2f for %s", filledOrder->totalOrderCost(), filledOrder->security.c_str());
        addPosition(filledOrder);
        m_liquidCash -= filledOrder->totalOrderCost();
        m_numTrades++;
    }

    m_orderCallbacks.at(filledOrder->uuid.hash())(filledOrder);
    m_allFilledOrders.emplace(filledOrder->uuid.hash(), filledOrder);
}

void Portfolio::addPosition(std::shared_ptr<FilledOrder> filledOrder)
{
    if (m_holdings.find(filledOrder->security) != m_holdings.end()) {
        Holding &holding = m_holdings.at(filledOrder->security);
        holding.m_positions.emplace_back(filledOrder);
        holding.numShares += filledOrder->numShares;
    } else {
        Holding holding(filledOrder->security);
        holding.m_positions.emplace_back(filledOrder);
        holding.numShares += filledOrder->numShares;
        m_holdings.emplace(std::move(holding.security), std::move(holding));
    }
}

// TODO: Support sell orders
void Portfolio::placeOrder(std::shared_ptr<Order> order)
{
    m_allOrders.emplace(order->uuid.hash(), order);
    std::shared_ptr<Event> event = std::make_shared<OrderEvent>(order);
    std::dynamic_pointer_cast<OrderEvent>(event)->callback = [this](auto &&PH1) { handleOrderEventConcluded(PH1); };
    std::dynamic_pointer_cast<OrderEvent>(event)->verifyPortfolioFunds = [this](float totalCost) {
        return verifyCapital(totalCost);
    };

    m_orderCallbacks.emplace(order->uuid.hash(), order->strategyCallback);
    m_exchange.addEvent(event);
}

[[nodiscard]] bool Portfolio::verifyCapital(const float totalCost) const
{
    return m_liquidCash >= totalCost;
}

void Portfolio::handleMarketData(const std::unordered_map<std::string, lud::CandlestickData> &data)
{
    updateHistoric(data);
}

void Portfolio::updateHistoric(const CandlestickDataMap &data)
{
    float worth = 0;
    for (const auto &holding : m_holdings) {
        worth += holding.second.numShares * data.at(holding.first).close;
    }
    m_portfolioValue = worth;
}

void Portfolio::summary() const
{
    std::cout << "Portfolio Summary" << std::endl;
    std::cout << "\tBalance: " << m_portfolioValue + m_liquidCash << std::endl;
    std::cout << "\tReturn: " << (m_portfolioValue + m_liquidCash) / m_startingCapital * 100 << "%" << std::endl;
}

}
