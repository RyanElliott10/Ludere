//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_ENGINE_HPP
#define LUDERE_ENGINE_HPP


#include <unordered_set>

#include <ludere/exchange.hpp>
#include <ludere/abstract_strategy.hpp>

namespace lud {

class engine
{
public:
    engine(exchange &exchange_, std::unordered_set<std::shared_ptr<abstract_strategy>> &strategies_);

    void trade();

private:
    bool m_is_trading;
    exchange &m_exchange;
    std::unordered_set<std::shared_ptr<abstract_strategy>> &m_strategies;
};

}


#endif //LUDERE_ENGINE_HPP
