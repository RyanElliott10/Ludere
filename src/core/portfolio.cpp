//
// Created by Ryan Elliott on 7/30/20.
//

#include <ludere/portfolio.hpp>

namespace lud {

portfolio::portfolio(exchange &exchange_, const float cash_)
        : m_exchange(exchange_), m_starting_capital(cash_), m_liquid_cash(cash_), m_portfolio_value(cash_), m_num_trades(0)
{}

void portfolio::handleOrderEventConcluded(std::shared_ptr<filled_order> &filled_order_)
{
    if (filled_order_->m_order_status == filled_order::filled_order_statuses::SUCCESS) {
        LUD_DEBUG("Total order cost: $%.2f for %s", filled_order_->total_order_cost(), filled_order_->m_security.c_str());
        addPosition(filled_order_);
        m_liquid_cash -= filled_order_->total_order_cost();
        m_num_trades++;
    }

    m_order_callbacks.at(filled_order_->m_uuid.hash())(filled_order_);
    m_all_filled_orders.emplace(filled_order_->m_uuid.hash(), filled_order_);
}

void portfolio::addPosition(std::shared_ptr<filled_order> filledOrder)
{
    if (m_holdings.find(filledOrder->m_security) != m_holdings.end()) {
        holding &holding_ = m_holdings.at(filledOrder->m_security);
        holding_.m_positions.emplace_back(filledOrder);
        holding_.m_num_shares += filledOrder->m_num_shares;
    } else {
        holding holding_(filledOrder->m_security);
        holding_.m_positions.emplace_back(filledOrder);
        holding_.m_num_shares += filledOrder->m_num_shares;
        m_holdings.emplace(std::move(holding_.m_security), std::move(holding_));
    }
}

// TODO: Support sell orders
void portfolio::placeOrder(std::shared_ptr<order> order_)
{
    m_all_orders.emplace(order_->m_uuid.hash(), order_);
    std::shared_ptr<event> event_ = std::make_shared<order_event>(order_);
    std::dynamic_pointer_cast<order_event>(event_)->m_callback = [this](auto &&PH1) { handleOrderEventConcluded(PH1); };
    std::dynamic_pointer_cast<order_event>(event_)->m_verify_portfolio_funds = [this](float total_cost_) {
        return verifyCapital(total_cost_);
    };

    m_order_callbacks.emplace(order_->m_uuid.hash(), order_->m_strategy_callback);
    m_exchange.addEvent(event_);
}

[[nodiscard]] bool portfolio::verifyCapital(const float total_cost_) const
{
    return m_liquid_cash >= total_cost_;
}

void portfolio::handle_market_data(const std::unordered_map<std::string, lud::candlestick_data> &data_)
{
    update_historic(data_);
}

void portfolio::update_historic(const candlestick_data_map &data_)
{
    float worth_ = 0;
    for (const auto &holding_ : m_holdings) {
        worth_ += holding_.second.m_num_shares * data_.at(holding_.first).m_close;
    }
    m_portfolio_value = worth_;
}

void portfolio::summary() const
{
    std::cout << "Portfolio Summary" << std::endl;
    std::cout << "\tBalance: " << m_portfolio_value + m_liquid_cash << std::endl;
    std::cout << "\tReturn: " << (m_portfolio_value + m_liquid_cash) / m_starting_capital * 100 << "%" << std::endl;
}

}
