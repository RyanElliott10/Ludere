//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_ORDER_HPP
#define LUDERE_ORDER_HPP


#include <string>

#include <ludere/UUID.hpp>

namespace lud {

enum class OrderType
{
    kMarket = 0,
    kLimit
};

struct MarketOrder
{
    float marketPrice;
    float funds;

    bool isAcceptableTrade(float price) { return price <= marketPrice && price <= funds; }
};

struct LimitOrder
{
    float funds;

    bool isAcceptableTrade(float price) { return price <= funds; }
};

class Order
{
public:
    std::string security;
    OrderType orderType;
    MarketOrder &marketOrder;
    LimitOrder &limitOrder;
    std::unique_ptr<UUID> id = UUID::generateUUID();
};

}


#endif //LUDERE_ORDER_HPP
