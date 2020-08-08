//
// Created by Ryan Elliott on 7/30/20.
//

#include <iostream>
#include <fstream>

#include <ludere/CandlestickData.hpp>
#include <ludere/Exchange.hpp>
#include <ludere/FillEvent.hpp>


namespace lud {

Exchange::Exchange(const std::string &dataFilename)
        : m_dataFilename(dataFilename)
{
    m_isTrading = false;
}

Exchange::Exchange(const std::string &dataFilename, const bool invertedDataStream)
        : m_dataFilename(dataFilename), m_invertedDataStream(invertedDataStream)
{
    m_isTrading = false;
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
        // Dispatch async?
        for (const std::shared_ptr<IDataEventSubscriber> &subscriber : m_dataEventSubscribers) {
            subscriber->handleMarketData(candle);
        }

        while (!m_eventQueue.empty()) {
            std::shared_ptr<Event> event = m_eventQueue.front();
            switch (event->type) {
            case EventType::kMarketEvent: {
                auto &strictEvent = dynamic_cast<MarketEvent &>(*event);
                for (const std::shared_ptr<IMarketEventSubscriber> &subscriber : m_marketEventSubscribers) {
                    subscriber->notifyOfMarketEvent(strictEvent);
                }
                break;
            }
            case EventType::kFillEvent: {
                auto &strictEvent = dynamic_cast<FillEvent &>(*event);
                // ... fill the order
                break;
            }
            }

            m_eventQueue.pop();
        }
    }
}

void Exchange::addEvent(std::shared_ptr<Event> event)
{
    m_eventQueue.push(std::move(event));
}

void Exchange::subscribeToDataStream(const std::shared_ptr<IDataEventSubscriber> &subscriber)
{
    m_dataEventSubscribers.push_back(subscriber);
}

}
