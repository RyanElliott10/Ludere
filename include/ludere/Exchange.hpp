//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EXCHANGE_HPP
#define LUDERE_EXCHANGE_HPP


#include <list>
#include <vector>

#include <ludere/Core.hpp>
#include <ludere/IDataEventSubscriber.hpp>
#include <ludere/IDataStreamer.hpp>
#include <ludere/IMarketEventSubscriber.hpp>
#include <ludere/OrderEvent.hpp>

namespace lud {

class Exchange
{
public:
    explicit Exchange(std::shared_ptr<IDataStreamer> dataStream);
    Exchange(std::shared_ptr<IDataStreamer> dataStream, bool invertedDataStream);

    /// Initiates trading on the Exchange and informs all Strategies trading has begun.
    void trade();

    /// Handles queuing of events.
    void addEvent(std::shared_ptr<Event> &event);

    /// Used to subscribe a Strategy to an Exchange's datastream.
    void subscribeToDataStream(const std::shared_ptr<DataEventSubscription> &subscription);

private:
    bool m_isTrading;
    EventQueue m_eventQueue;
    std::list<std::shared_ptr<OrderEvent>> m_orderQueue;
    const bool m_invertedDataStream = false;
    std::shared_ptr<IDataStreamer> m_dataStream;

    // Subscribers
    std::vector<const std::shared_ptr<DataEventSubscription>> m_dataEventSubscribers;
    std::vector<const std::shared_ptr<IMarketEventSubscriber>> m_marketEventSubscribers;

private:
    void streamData(const CandlestickDataMap &data);
    void handleOrderEvent(const std::shared_ptr<Event> &event, const CandlestickDataMap &candles);
    void handleMarketEvent(const std::shared_ptr<Event> &event);
    void handleLimitOrder(const std::shared_ptr<LimitOrder> &order, const CandlestickDataMap &candles, std::list<std::shared_ptr<OrderEvent>>::iterator &it);
    void handleMarketOrder(const std::shared_ptr<MarketOrder> &order, const CandlestickDataMap &candles, std::list<std::shared_ptr<OrderEvent>>::iterator &it);
};

}


#endif //LUDERE_EXCHANGE_HPP
