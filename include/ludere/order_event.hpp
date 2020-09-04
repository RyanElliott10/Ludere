//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_ORDEREVENT_HPP
#define LUDERE_ORDEREVENT_HPP


#include <boost/function.hpp>

#include <ludere/event.hpp>
#include <ludere/filled_order.hpp>
#include <ludere/order.hpp>

namespace lud {

class order_event : public event
{
public:
  explicit order_event(std::shared_ptr<order> order_)
          : m_order(std::move(order_))
  {
    m_type = event_type::ORDER;
  }

public:
  std::shared_ptr<order> m_order;
  boost::function<void(std::shared_ptr<filled_order>)> m_callback;
  boost::function<bool(const std::shared_ptr<order> &, const float)> m_verify_portfolio_funds;
  boost::function<bool(const int, const std::string &)> m_verify_portfolio_shares;
};

}


#endif //LUDERE_ORDEREVENT_HPP
