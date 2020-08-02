//
// Created by Ryan Elliott on 7/30/20.
//

#include <ludere/Portfolio.hpp>

namespace lud {

Portfolio::Portfolio(Exchange &exchange, float value, float cash)
        : m_exchange(exchange), m_portfolioValue(value), m_liquidCash(cash), m_numTrades(0)
{

}

}
