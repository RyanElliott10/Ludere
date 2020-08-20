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

/// Duration an order should remain active
struct OrderLifetime
{
    enum class Duration
    {
        kHour = 3600,
        kDay = 3600 * 24,
        kWeek = 3600 * 24 * 7
    };

    explicit OrderLifetime(Duration type, time_t baseTime_)
            : duration(int(type)), baseTime(baseTime_)
    {}

    explicit OrderLifetime(time_t customDuration, time_t baseTime_)
            : duration(customDuration), baseTime(baseTime_)
    {}

    /// Duration, in seconds
    time_t duration;
    time_t baseTime;
};

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

    Order(std::string security_, uint32_t numShares_, OrderType orderType_, PositionType positionType_,
          OrderLifetime orderLifetime_, StrategyCallbackDef callback_)
            : security(std::move(security_)), numShares(numShares_), orderType(orderType_), positionType(positionType_),
              orderLifetime(orderLifetime_), strategyCallback(std::move(callback_)), uuid(UUID())
    {}

    [[nodiscard]] inline bool isExpired(time_t currentTime) const
    {
        return (orderLifetime.baseTime + orderLifetime.duration) < currentTime;
    }

    virtual ~Order() = default;

    virtual float maxOrderCost() = 0;

    std::string security;
    uint32_t numShares;
    PositionType positionType;
    OrderType orderType;
    OrderLifetime orderLifetime;
    StrategyCallbackDef strategyCallback;
    UUID uuid;
};

struct LimitOrder : public Order
{
    LimitOrder(std::string security, uint32_t numShares, PositionType positionType, float limitPrice_,
               OrderLifetime orderLifetime_, StrategyCallbackDef callback_)
            : Order(std::move(security), numShares, OrderType::kLimitOrder, positionType, orderLifetime_,
                    std::move(callback_)),
              limitPrice(limitPrice_)
    {}

    inline float maxOrderCost() override
    { return limitPrice * numShares; }

    float limitPrice;
};

struct MarketOrder : public Order
{
    MarketOrder(std::string security, uint32_t numShares, PositionType positionType, float marketPrice_,
                OrderLifetime orderLifetime_, StrategyCallbackDef callback_)
            : Order(std::move(security), numShares, OrderType::kMarketOrder, positionType, orderLifetime_,
                    std::move(callback_)),
              marketPrice(marketPrice_)
    {}

    // TODO: Remove usage of marketPrice since MarketOrders don't have a marketPrice field, they simply execute at the
    //      highest price on the market for maximum liquidity
    inline float maxOrderCost() override
    { return marketPrice * numShares; }

    float marketPrice;
};

inline std::ostream &operator<<(std::ostream &strm, const LimitOrder &order)
{
    return strm << order.security << " " << order.numShares << " @ $" << order.limitPrice;
}

inline std::ostream &operator<<(std::ostream &strm, const Order &order)
{
    return strm << order.security;
}

inline std::ostream &operator<<(std::ostream &strm, const MarketOrder &order)
{
    return strm << order.security << " " << order.numShares << " @ $" << order.marketPrice;
}

}


#endif //LUDERE_ORDER_HPP
