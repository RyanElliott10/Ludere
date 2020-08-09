//
// Created by Ryan Elliott on 7/30/20.
//

#include <fstream>

#include <ludere/CandlestickData.hpp>
#include <ludere/Exchange.hpp>
#include <ludere/FillEvent.hpp>
#include <ludere/FilledOrder.hpp>


namespace lud {

Exchange::Exchange(const std::string &dataFilename)
        : m_dataFilename(dataFilename), m_isTrading(false)
{}

Exchange::Exchange(const std::string &dataFilename, const bool invertedDataStream)
        : m_dataFilename(dataFilename), m_invertedDataStream(invertedDataStream), m_isTrading(false)
{
    if (m_invertedDataStream) {
        LD_ERROR("Inverted datastream is not yet supported", nullptr);
        exit(-1);
    }
}

void Exchange::trade()
{
    m_isTrading = true;

    std::ifstream file(m_dataFilename);
    lud::CandlestickData candle;
    while (file >> candle) {
        for (const auto &subscriber : m_dataEventSubscribers) {
            subscriber->handleMarketData(candle);
        }

        while (!m_eventQueue.empty()) {
            std::shared_ptr<Event> event = m_eventQueue.front();
            switch (event->type) {
            case EventType::kFillEvent: {
                handleFillEvent(event);
                break;
            }
            case EventType::kMarketEvent: {
                handleMarketEvent(event);
                break;
            }
            }

            m_eventQueue.pop();
        }
    }
}

void Exchange::handleFillEvent(std::shared_ptr<Event> &event)
{
    auto &fillEvent = dynamic_cast<FillEvent &>(*event);
    if (fillEvent.verifyPortfolioFunds(fillEvent.order->maxOrderCost())) {
        std::shared_ptr<Order> order = fillEvent.order;
        FilledOrder filledOrder(order->security, order->numShares, order->maxOrderCost() / order->numShares,
                                FilledOrder::FilledOrderStatus::kSuccess);
        fillEvent.callback(filledOrder);
    }
}

void Exchange::handleMarketEvent(std::shared_ptr<Event> &event)
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

void Exchange::subscribeToDataStream(const std::shared_ptr<IDataEventSubscriber> &subscriber)
{
    m_dataEventSubscribers.push_back(subscriber);
}

}
