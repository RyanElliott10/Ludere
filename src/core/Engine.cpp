//
// Created by Ryan Elliott on 7/30/20.
//

#include <ludere/Engine.hpp>
#include <ludere/Log.hpp>

namespace lud {

Engine::Engine(Exchange &exchange, std::unordered_set<std::unique_ptr<AbstractStrategy>> &strategies)
        : m_exchange(exchange), m_strategies(strategies)
{
    LD_DEBUG("Creating instance of lud::Engine", nullptr);
    m_isTrading = false;
}

void Engine::trade()
{
    m_isTrading = true;
    for (const auto & m_strategy : m_strategies) {
        m_strategy->trade();
    }
    m_exchange.trade();
}

}