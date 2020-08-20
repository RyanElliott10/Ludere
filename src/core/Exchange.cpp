//
// Created by Ryan Elliott on 7/30/20.
//

#include <utility>

#include <ludere/CandlestickData.hpp>
#include <ludere/Exchange.hpp>
#include <ludere/FilledOrder.hpp>


namespace lud {

Exchange::Exchange(std::shared_ptr<IDataStreamer> dataStream)
        : m_dataStream(std::move(dataStream)), m_isTrading(false)
{}

Exchange::Exchange(std::shared_ptr<IDataStreamer> dataStream, const bool invertedDataStream)
        : m_dataStream(std::move(dataStream)), m_invertedDataStream(invertedDataStream), m_isTrading(false)
{
    if (m_invertedDataStream) {
        LUD_ERROR("Inverted datastream is not yet supported", nullptr);
        exit(-1);
    }
}

void Exchange::trade()
{
    m_isTrading = true;

    CandlestickDataMap candles;
    while (!(candles = m_dataStream->pollNextStream()).empty()) {
        streamData(candles);
        while (!m_eventQueue.empty()) {
            std::shared_ptr<Event> event = m_eventQueue.front();
            switch (event->type) {
            case EventType::kOrderEvent:
                handleOrderEvent(event, candles);
                break;
            case EventType::kMarketEvent:
                handleMarketEvent(event);
                break;
            }

            m_eventQueue.pop();
        }
    }
}

void Exchange::streamData(const CandlestickDataMap &data)
{
    for (const auto &subscriber : m_dataEventSubscribers) {
        subscriber->ref->handleMarketData(data);
    }
}

void Exchange::handleOrderEvent(const std::shared_ptr<Event> &event, const CandlestickDataMap &candles)
{
    std::shared_ptr<OrderEvent> orderEvent = std::dynamic_pointer_cast<OrderEvent>(event);
    m_orderQueue.push_back(orderEvent);
    for (auto it = m_orderQueue.begin(); it != m_orderQueue.end(); it++) {
        Order::OrderType &type = (*it)->order->orderType;
        if (type == Order::OrderType::kLimitOrder) {
            handleLimitOrder(std::dynamic_pointer_cast<LimitOrder>((*it)->order), candles, it);
        } else if (type == Order::OrderType::kMarketOrder) {
            handleMarketOrder(std::dynamic_pointer_cast<MarketOrder>((*it)->order), candles, it);
        }
    }
}

// TODO: Implement slippage and more realistic market conditions. This is guaranteed to fill your order at the best
//      price and immediately (as long as the security's price is below the limit)
void Exchange::handleLimitOrder(const std::shared_ptr<LimitOrder> &order, const CandlestickDataMap &candles,
                                std::list<std::shared_ptr<OrderEvent>>::iterator &it)
{
    const float securityPrice = candles.at((*it)->order->security).close;
    if (order->limitPrice >= securityPrice && (*it)->verifyPortfolioFunds(order->maxOrderCost())) {
        std::unique_ptr<FilledOrder> filledOrder = std::make_unique<FilledOrder>(order->security, order->numShares,
                                                                                 securityPrice,
                                                                                 FilledOrder::FilledOrderStatus::kSuccess,
                                                                                 order->uuid);
        (*it)->callback(std::move(filledOrder));
        it = m_orderQueue.erase(it);
    }
}

void Exchange::handleMarketOrder(const std::shared_ptr<MarketOrder> &order, const CandlestickDataMap &candles,
                                 std::list<std::shared_ptr<OrderEvent>>::iterator &it)
{

}

void Exchange::handleMarketEvent(const std::shared_ptr<Event> &event)
{
    auto &marketEvent = dynamic_cast<MarketEvent &>(*event);
    for (const auto &subscriber : m_marketEventSubscribers) {
        subscriber->notifyOfMarketEvent(marketEvent);
    }
}

void Exchange::addEvent(std::shared_ptr<Event> &event)
{
    m_eventQueue.push(event);
}

void Exchange::subscribeToDataStream(const std::shared_ptr<DataEventSubscription> &subscription)
{
    m_dataEventSubscribers.push_back(subscription);
}

}
