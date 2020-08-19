//
// Created by Ryan Elliott on 7/30/20.
//

#include <ludere/Portfolio.hpp>

namespace lud {

Portfolio::Portfolio(Exchange &exchange, const float cash)
        : m_exchange(exchange), m_liquidCash(cash), m_portfolioValue(cash), m_numTrades(0)
{}

// TODO: Aggregate holdings per security. If a user buys 2 AAPL and then 5 AAPL, the Portfolio should view that as 7
//      AAPL rather than 2 and 5. Further, there should be information about entry and exit (average price, etc.)
void Portfolio::handleOrderEventConcluded(std::shared_ptr<FilledOrder> &filledOrder)
{
    if (filledOrder->orderStatus == FilledOrder::FilledOrderStatus::kSuccess) {
        LUD_DEBUG("Total order cost: $%.2f for %s", filledOrder->totalOrderCost(), filledOrder->security.c_str());
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
    std::shared_ptr<Event> event = std::make_shared<OrderEvent>(order);
    std::dynamic_pointer_cast<OrderEvent>(event)->callback = [this](auto &&PH1) { handleOrderEventConcluded(PH1); };
    std::dynamic_pointer_cast<OrderEvent>(event)->verifyPortfolioFunds = [this](float totalCost) {
        return verifyCapital(totalCost);
    };

    m_orderCallbacks.emplace(order->uuid.hash(), order->strategyCallback);
    m_exchange.addEvent(event);
}

bool Portfolio::verifyCapital(const float totalCost) const
{
    return m_liquidCash >= totalCost;
}

void Portfolio::handleMarketData(const std::unordered_map<std::string, lud::CandlestickData> &data)
{
    updateHistoric(data);
}

/**
 * Updates the portfolio's historic value against a CandlestickData instance.
 * TODO: Perhaps subscribe a portfolio to a datastream. Further, support requests for a specific subset of securities.
 *      If a Portfolio is only interested in Apple, TSLA, and SPY, it should only receive information about those
 *      securities. In other words, plan out the complete data streaming flow and capabilities.
 *      In this case, the data should be an unordered_map. Allow the Portfolio to query the data for specific tickers.
 * @param data
 */
void Portfolio::updateHistoric(const CandlestickDataMap &data)
{
    float worth = 0;
    for (const auto &position : m_positions) {
        worth += position.filledOrder->numShares * data.at(position.filledOrder->security).close;
    }
    m_portfolioValue = worth;
}

}
