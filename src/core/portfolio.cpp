//
// Created by Ryan Elliott on 7/30/20.
//

#include <locale>

#include <ludere/portfolio.hpp>

namespace lud {

portfolio::portfolio(exchange &exchange_, const float cash_)
        : m_exchange(exchange_), m_starting_capital(cash_), m_liquid_cash(cash_), m_portfolio_value(cash_),
          m_num_trades(0)
{}

void portfolio::handle_order_event_concluded(const std::shared_ptr<order> &order_,
                                             std::shared_ptr<filled_order> &filled_order_)
{
    switch (filled_order_->m_order_signal) {
    case enums::order::signals::BUY:
        handle_buy_order_event_concluded(order_, filled_order_);
        break;
    case enums::order::signals::SELL:
        handle_sell_order_event_concluded(order_, filled_order_);
        break;
    }

    m_order_callbacks.at(filled_order_->m_uuid.hash())(filled_order_);
    m_all_filled_orders.emplace(filled_order_->m_uuid.hash(), filled_order_);
}

void portfolio::handle_buy_order_event_concluded(const std::shared_ptr<order> &order_,
                                                 std::shared_ptr<filled_order> &filled_order_)
{
    if (filled_order_->m_order_status == enums::order::fill_statuses::SUCCESS) {
        LUD_DEBUG("Buy order executed: %d @ $%.2f for %s", filled_order_->m_num_shares,
                  filled_order_->total_order_cost(), filled_order_->m_security.c_str());
        add_position(order_, filled_order_);
        m_liquid_cash -= filled_order_->total_order_cost();
        m_liquid_cash -= m_brokerage_fees.fees_for_order(filled_order_);
        m_num_trades++;
    }
}

void portfolio::handle_sell_order_event_concluded(const std::shared_ptr<order> &order_,
                                                  std::shared_ptr<filled_order> &filled_order_)
{
    if (filled_order_->m_order_status == enums::order::fill_statuses::SUCCESS) {
        LUD_DEBUG("Sell order executed: %d @ $%.2f for %s", filled_order_->m_num_shares, filled_order_->m_share_price,
                  filled_order_->m_security.c_str());
        add_position(order_, filled_order_);
        m_liquid_cash += filled_order_->total_order_cost();
        m_liquid_cash -= m_brokerage_fees.fees_for_order(filled_order_);
        m_num_trades++;
    }
}

void portfolio::add_position(std::shared_ptr<order> order_, std::shared_ptr<filled_order> filled_order_)
{
    switch (filled_order_->m_order_signal) {
    case enums::order::signals::BUY:
        if (m_holdings.find(filled_order_->m_security) != m_holdings.end()) {
            holding &holding_ = m_holdings.at(filled_order_->m_security);
            holding_.m_positions.emplace_back(order_, filled_order_);
            holding_.m_num_shares += filled_order_->m_num_shares;
        } else {
            holding holding_(filled_order_->m_security);
            holding_.m_positions.emplace_back(order_, filled_order_);
            holding_.m_num_shares += filled_order_->m_num_shares;
            m_holdings.emplace(std::move(holding_.m_security), std::move(holding_));
        }
        break;
    case enums::order::signals::SELL:
        holding &holding_ = m_holdings.at(filled_order_->m_security);
        holding_.m_positions.emplace_back(order_, filled_order_);
        holding_.m_num_shares -= filled_order_->m_num_shares;
        break;
    }
}

void portfolio::place_order(std::shared_ptr<order> order_)
{
    m_all_orders.emplace(order_->m_uuid.hash(), order_);
    std::shared_ptr<event> event_ = std::make_shared<order_event>(order_);
    std::dynamic_pointer_cast<order_event>(event_)->m_callback = [this, order_](auto &&PH1) {
        handle_order_event_concluded(order_, PH1);
    };
    if (order_->m_order_signal == enums::order::signals::BUY) {
        std::dynamic_pointer_cast<order_event>(event_)->m_verify_portfolio_funds = [this](
                const std::shared_ptr<order> &order_, const float security_price_) {
            return verify_capital(order_, security_price_);
        };
    } else if (order_->m_order_signal == enums::order::signals::SELL) {
        std::dynamic_pointer_cast<order_event>(event_)->m_verify_portfolio_shares = [this](const int num_shares_,
                                                                                           const std::string &security_) {
            return verify_num_shares(num_shares_, security_);
        };
    }

    m_order_callbacks.emplace(order_->m_uuid.hash(), order_->m_strategy_callback);
    m_exchange.add_event(event_);
}

[[nodiscard]] bool portfolio::verify_capital(const std::shared_ptr<order> &order_, const float security_price_) const
{
    float brokerage_fees_ = m_brokerage_fees.fees_for_order(order_);
    float total_cost_ = brokerage_fees_ + (order_->m_num_shares * security_price_);
    return m_liquid_cash >= total_cost_;
}

[[nodiscard]] bool portfolio::verify_num_shares(const int num_shares_, const std::string &security_) const
{
    if (m_holdings.find(security_) != m_holdings.end()) {
        return m_holdings.at(security_).m_num_shares >= num_shares_;
    }
    return false;
}

void portfolio::handle_market_data(const lud::candlestick_data_aggregate &data_)
{
    update_historic(data_);
}

void portfolio::update_historic(const candlestick_data_aggregate &data_)
{
    float worth_ = 0;
    for (const auto &holding_ : m_holdings) {
        worth_ += (float) holding_.second.m_num_shares * data_.at(holding_.first).m_close;
    }
    m_portfolio_value = worth_;
}

// TODO: Add commas to portfolio values
void portfolio::summary() const
{
    std::cout.imbue(std::locale(""));
    std::cout << 100000 << std::endl;
    std::cout << "Portfolio Summary" << std::endl;
    std::cout << "\tBalance: $" << m_portfolio_value + m_liquid_cash << std::endl;
    std::cout << "\tReturn: " << (m_portfolio_value + m_liquid_cash - m_starting_capital) / m_starting_capital * 100
              << "%" << std::endl;
    std::cout << "\tNumber Trades: " << m_num_trades << std::endl;
    std::cout << "\tBrokerage Fees: " << std::endl;
}

}
