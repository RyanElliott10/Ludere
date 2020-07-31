//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_PORTFOLIO_HPP
#define LUDERE_PORTFOLIO_HPP


#include <ludere/Exchange.hpp>

namespace lud {

class Portfolio
{
public:
    Portfolio(Exchange &exchange);

private:
    Exchange &m_exchange;
    float m_portfolioValue;
    float m_liquidCash;
    int m_numTrades;
    // std::vector<History> &m_history;
};

}


#endif //LUDERE_PORTFOLIO_HPP
