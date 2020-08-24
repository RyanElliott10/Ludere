//
// Created by Ryan Elliott on 7/30/20.
//

#include <ludere/engine.hpp>
#include <ludere/log.hpp>

namespace lud {

engine::engine(exchange &exchange_, std::unordered_set<std::shared_ptr<abstract_strategy>> &strategies_)
        : m_exchange(exchange_), m_strategies(strategies_)
{
    LUD_DEBUG("Creating instance of lud::Engine", nullptr);
    m_is_trading = false;
}

void engine::trade()
{
    m_is_trading = true;
    for (const auto &strategy_ : m_strategies) {
        strategy_->trade();
    }
    m_exchange.trade();
}

}
