//
// Created by Ryan Elliott on 7/30/20.
//

#include <ludere/Engine.hpp>
#include <ludere_private/Log.hpp>

namespace lud {

Engine::Engine(Exchange &exchange, std::unordered_set<std::unique_ptr<Strategy>> &strategies)
        : m_exchange(exchange), m_strategies(strategies)
{
    LD_DEBUG("Creating instance of lud::Engine", nullptr);
    m_isTrading = false;
}

void Engine::beingTrading()
{
    m_isTrading = true;
    m_exchange.beginTrading();
    for (auto i = m_strategies.begin(); i != m_strategies.end(); i++) {
        (*i)->beginTrading();
    }
}

}