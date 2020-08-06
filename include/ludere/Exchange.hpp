//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EXCHANGE_HPP
#define LUDERE_EXCHANGE_HPP


#include <vector>

#include <ludere/Core.hpp>
#include <ludere/IMarketEventSubscriber.hpp>

namespace lud {

class Exchange
{
public:
    Exchange(const std::string &dataFilename);

    Exchange(const std::string &dataFilename, const bool invertedDatastream);

    void trade();

    void addEvent(std::unique_ptr<Event> event);

private:
    bool m_isTrading;
    EventQueue m_eventQueue;
    std::vector<std::unique_ptr<IMarketEventSubscriber>> m_marketEventSubscribers;
    const std::string &m_dataFilename;
    const bool m_invertedDatastream = false;
};

}


#endif //LUDERE_EXCHANGE_HPP
