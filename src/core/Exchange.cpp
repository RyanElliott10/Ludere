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

// TODO: Develop a more robust order-filling mechanism.
void Exchange::handleOrderEvent(const std::shared_ptr<Event> &event, const CandlestickDataMap &candles)
{
    std::shared_ptr<OrderEvent> orderEvent = std::dynamic_pointer_cast<OrderEvent>(event);
    m_orderQueue.push_back(orderEvent);
    for (auto it = m_orderQueue.begin(); it != m_orderQueue.end(); it++) {
        // TODO: Verify the current data against the prices the Order is requesting
        if ((*it)->verifyPortfolioFunds(orderEvent->order->maxOrderCost())) {
            auto order = orderEvent->order;
            std::unique_ptr<FilledOrder> filledOrder = std::make_unique<FilledOrder>(order->security, order->numShares,
                                                                                     order->maxOrderCost() /
                                                                                     order->numShares,
                                                                                     FilledOrder::FilledOrderStatus::kSuccess,
                                                                                     orderEvent->order->uuid);
            orderEvent->callback(std::move(filledOrder));
            it = m_orderQueue.erase(it);
        }
    }
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
