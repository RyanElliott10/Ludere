//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_ENGINE_HPP
#define LUDERE_ENGINE_HPP


#include <unordered_set>

#include <ludere/Exchange.hpp>
#include <ludere/AbstractStrategy.hpp>

namespace lud {

class Engine
{
public:
    Engine(Exchange &exchange, std::unordered_set<std::unique_ptr<AbstractStrategy>> &strategies);

    void trade();

private:
    bool m_isTrading;
    Exchange &m_exchange;
    std::unordered_set<std::unique_ptr<AbstractStrategy>> &m_strategies;
};

}


#endif //LUDERE_ENGINE_HPP
