
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_PORTFOLIO_HPP
#define LUDERE_PORTFOLIO_HPP


#include <unordered_map>

#include <boost/function.hpp>

#include <ludere/brokerage_fees.hpp>
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
  portfolio(exchange &exchange_, float cash_);

  inline void set_brokerage_fees(brokerage_fees brokerage_fees_)
  {
    m_brokerage_fees = brokerage_fees_;
  }

  void handle_buy_order_event_concluded(std::shared_ptr<filled_order> &filled_order_);
  void handle_sell_order_event_concluded(std::shared_ptr<filled_order> &filled_order_);
  void place_order(std::shared_ptr<order> order_);

  /// Verify a Portfolio has enough capital to perform a trade.
  [[nodiscard]] bool verify_capital(const std::shared_ptr<order> &order_, const float security_price_) const;

  /// Ensures the Portfolio has at least num_shares_ for security_.
  [[nodiscard]] bool verify_num_shares(int num_shares_, const std::string &security_) const;

  /// Ensures the Portfolio is liquid enough for a given cost. Does not natively incorporate brokerage fees.
  [[nodiscard]] bool soft_verify_capital(const float cost_) const
  { return m_liquid_cash >= cost_; }

  [[nodiscard]] bool soft_verify_shares(const std::string &ticker_, const float num_shares_) const
  { return m_holdings.find(ticker_) != m_holdings.end() && m_holdings.at(ticker_).m_num_shares >= num_shares_; }

  void handle_market_data(const lud::candlestick_data_aggregate &data_) override;

  /// Update the Portfolio's balance against the current positions and current market data
  void update_historic(const candlestick_data_aggregate &data_);

  [[nodiscard]] inline float net_value() const
  { return m_portfolio_value + m_liquid_cash; }

  void summary() const;

private:
  exchange &m_exchange;
  const float m_starting_capital;
  float m_portfolio_value;
  float m_liquid_cash;
  int m_num_trades;

  brokerage_fees m_brokerage_fees;

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
