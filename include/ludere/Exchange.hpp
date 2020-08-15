//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EXCHANGE_HPP
#define LUDERE_EXCHANGE_HPP


#include <vector>

#include <ludere/Core.hpp>
#include <ludere/IDataEventSubscriber.hpp>
#include <ludere/IMarketEventSubscriber.hpp>

namespace lud {

class Exchange
{
public:
    explicit Exchange(const std::string &dataFilename);
    Exchange(const std::string &dataFilename, bool invertedDataStream);

    void trade();
    void addEvent(std::shared_ptr<Event> &event);
    void subscribeToDataStream(const std::shared_ptr<DataEventSubscription> subscription);

private:
    bool m_isTrading;
    EventQueue m_eventQueue;
    const std::string &m_dataFilename;
    const bool m_invertedDataStream = false;

    // Subscribers
    std::vector<const std::shared_ptr<DataEventSubscription>> m_dataEventSubscribers;
    std::vector<const std::shared_ptr<IMarketEventSubscriber>> m_marketEventSubscribers;

private:
    void streamData(CandlestickData &data);
    void handleFillEvent(std::shared_ptr<Event> &event);
    void handleMarketEvent(std::shared_ptr<Event> &event);
};

}


#endif //LUDERE_EXCHANGE_HPP
