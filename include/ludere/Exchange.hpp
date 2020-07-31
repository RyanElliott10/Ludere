//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_EXCHANGE_HPP
#define LUDERE_EXCHANGE_HPP


#include <vector>

#include <ludere_private/Core.hpp>
#include <ludere/CandlestickData.hpp>

namespace lud {

class Exchange
{
public:
    Exchange();

    void beginTrading();

private:
    bool m_isTrading;
    EventQueue m_eventQueue;
    std::vector<CandlestickCandle> m_candlestickFeed;
};

}


#endif //LUDERE_EXCHANGE_HPP
