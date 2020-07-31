//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_ENGINE_HPP
#define LUDERE_ENGINE_HPP


#include <unordered_set>

#include <ludere/Exchange.hpp>
#include <ludere/Strategy.hpp>

namespace lud {

class Engine
{
public:
    Engine(Exchange &, std::unordered_set<Strategy> &);

    void beingTrading();

private:
    bool m_isTrading;
    Exchange &m_exchange;
    std::unordered_set<Strategy> &m_strategies;
};

}


#endif //LUDERE_ENGINE_HPP
