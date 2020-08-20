//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_ORDER_HPP
#define LUDERE_ORDER_HPP


#include <chrono>
#include <string>
#include <utility>
#include <iostream>

#include <boost/function.hpp>

#include <ludere/FilledOrder.hpp>
#include <ludere/UUID.hpp>

// TODO: Reconsider the raw data storage of the Order class (and it's subclasses). Consider breaking data into smaller
//      objects. Further reading: https://softwareengineering.stackexchange.com/questions/305313/avoiding-constructors-with-many-arguments

namespace lud {

using StrategyCallbackDef = boost::function<void(std::shared_ptr<FilledOrder>)>;

// TODO: Implement order lifetimes (this order is good til n). Execution current slow downs stem from too many Orders
//      being added to the OrderQueue that will never be filled and are weeks, months, years old.
struct Order
{
    enum class PositionType
    {
        kLongPosition,
        kShortPosition
    };

    enum class OrderType
    {
        kLimitOrder,
        kMarketOrder
    };

    Order(std::string security_, uint32_t numShares_, OrderType orderType_, PositionType positionType_, StrategyCallbackDef callback_)
            : security(std::move(security_)), numShares(numShares_), orderType(orderType_), positionType(positionType_), strategyCallback(std::move(callback_)),
              timestamp(std::chrono::system_clock::now()), uuid(UUID())
    {}

    virtual ~Order() = default;

    virtual float maxOrderCost() = 0;

    std::string security;
    uint32_t numShares;
    PositionType positionType;
    OrderType orderType;
    StrategyCallbackDef strategyCallback;
    std::chrono::system_clock::time_point timestamp;
    UUID uuid;
};

struct MarketOrder : public Order
{
    MarketOrder(std::string security, uint32_t numShares, PositionType positionType, float marketPrice_, StrategyCallbackDef callback_)
            : Order(std::move(security), numShares, OrderType::kMarketOrder, positionType, std::move(callback_)), marketPrice(marketPrice_)
    {}

    float maxOrderCost() override
    {
        return marketPrice * numShares;
    }

    float marketPrice;
};

struct LimitOrder : public Order
{
    LimitOrder(std::string security, uint32_t numShares, PositionType positionType, float limitPrice_, StrategyCallbackDef callback_)
            : Order(std::move(security), numShares, OrderType::kLimitOrder, positionType, std::move(callback_)), limitPrice(limitPrice_)
    {}

    float maxOrderCost() override
    {
        return limitPrice * numShares;
    }

    float limitPrice;
};

inline std::ostream &operator<<(std::ostream &strm, const Order &order)
{
    return strm << order.security;
}

inline std::ostream &operator<<(std::ostream &strm, const MarketOrder &order)
{
    return strm << order.security << " " << order.numShares << " @ $" << order.marketPrice;
}

inline std::ostream &operator<<(std::ostream &strm, const LimitOrder &order)
{
    return strm << order.security << " " << order.numShares << " @ $" << order.limitPrice;
}

}


#endif //LUDERE_ORDER_HPP
