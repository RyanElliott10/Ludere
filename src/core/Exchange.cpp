//
// Created by Ryan Elliott on 7/30/20.
//

#include <iostream>
#include <fstream>

#include <ludere/CandlestickData.hpp>
#include <ludere/DataRequestEvent.hpp>
#include <ludere/Exchange.hpp>
#include <ludere/FillEvent.hpp>


namespace lud {

Exchange::Exchange(const std::string &dataFilename)
        : m_dataFilename(dataFilename)
{
    m_isTrading = false;
}

Exchange::Exchange(const std::string &dataFilename, const bool invertedDatastream)
        : m_dataFilename(dataFilename), m_invertedDatastream(invertedDatastream)
{
    m_isTrading = false;
    if (m_invertedDatastream) {
        LD_ERROR("Inverted datastream is not yet supported", nullptr);
        exit(-1);
    }
}

void Exchange::trade()
{
    m_isTrading = true;
    /**
     * TODO: Figure out the remaining events that the Exchange should be in control of
     * Although, I suppose the Exchange should be in control of all the events in the market; that's the point of
     *  an exchange. The Portfolio, and the Strategies derived from it, simply send events to the Exchange. That being
     *  said, we need to figure out the other types of events our system will handle.
     *
     * MarketEvent
     * FillEvent
     * DataRequestEvent -- When a Strategy requests data, we return the candle
     * SignalEvent -- I'm not too sure what this event represents; it seems like something relevant to AbstractStrategy (read
     *  algorithms) and shouldn't be something in the Exchange?
     */

    std::ifstream file(m_dataFilename);
    lud::CandlestickData candle;
    while (file >> candle) {
        std::cout << candle << std::endl;

        while (!m_eventQueue.empty()) {
            std::unique_ptr<Event> event = std::move(m_eventQueue.front());
            switch (event->type) {
            case EventType::kMarketEvent: {
                auto &strictEvent = dynamic_cast<MarketEvent &>(*event);
                for (auto i = std::next(m_marketEventSubscribers.begin()); i != m_marketEventSubscribers.end(); i++) {
                    (*i)->notifyOfMarketEvent(strictEvent);
                }
                break;
            }
            case EventType::kFillEvent: {
                auto &strictEvent = dynamic_cast<FillEvent &>(*event);
                // ... fill the order
                break;
            }
            case EventType::kDataRequestEvent: {
                auto &strictEvent = dynamic_cast<DataRequestEvent &>(*event);
                strictEvent.callback(candle);
                break;
            }
            }

            m_eventQueue.pop();
        }
    }
}

void Exchange::addEvent(std::unique_ptr<Event> event)
{
    m_eventQueue.push(std::move(event));
}

}