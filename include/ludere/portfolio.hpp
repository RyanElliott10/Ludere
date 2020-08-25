
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_PORTFOLIO_HPP
#define LUDERE_PORTFOLIO_HPP


#include <unordered_map>

#include <boost/function.hpp>

#include <ludere/exchange.hpp>
#include <ludere/order_event.hpp>
#include <ludere/filled_order.hpp>
#include <ludere/data_event_subscribable.hpp>
#include <ludere/order.hpp>
#include <ludere/position.hpp>
#include <ludere/uuid.hpp>

namespace lud {

class portfolio : public data_event_subscribable
{
public:
    // TODO: Add brokerage fees
    portfolio(exchange &exchange_, float cash_);

    void handle_buy_order_event_concluded(std::shared_ptr<filled_order> &filled_order_);
    void handle_sell_order_event_concluded(std::shared_ptr<filled_order> &filled_order_);
    void place_order(std::shared_ptr<order> order_);

    /// Verify a Portfolio has enough capital to perform a trade.
    [[nodiscard]] bool verify_capital(float total_cost_) const;
    [[nodiscard]] bool verify_num_shares(int num_shares_, const std::string &security_) const;

    void handle_market_data(const std::unordered_map<std::string, lud::candlestick_data> &data_) override;

    /// Update the Portfolio's balance against the current positions and current market data
    void update_historic(const candlestick_data_map &data_);

    [[nodiscard]] inline float net_value() const { return m_portfolio_value + m_liquid_cash; }

    void summary() const;

private:
    exchange &m_exchange;
    const float m_starting_capital;
    float m_portfolio_value;
    float m_liquid_cash;
    int m_num_trades;
    std::unordered_map<std::string, holding> m_holdings;
    std::unordered_map<uuid_hash, strategy_callback_def> m_order_callbacks;
    std::unordered_map<uuid_hash, std::shared_ptr<order>> m_all_orders; // All orders, even cancelled and failures
    std::unordered_map<uuid_hash, std::shared_ptr<filled_order>> m_all_filled_orders;

private:
    void handle_order_event_concluded(std::shared_ptr<filled_order> &filled_order_);
    void add_position(std::shared_ptr<filled_order> filled_order_);
};

}


#endif //LUDERE_PORTFOLIO_HPP