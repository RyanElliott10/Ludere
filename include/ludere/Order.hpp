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

namespace lud {

using StrategyCallbackDef = boost::function<void(std::shared_ptr<FilledOrder>)>;

struct Order
{
    enum class PositionType
    {
        kLongPosition,
        kShortPosition
    };

    Order(std::string security_, uint32_t numShares_, PositionType positionType_, StrategyCallbackDef callback_)
            : security(std::move(security_)), numShares(numShares_), positionType(positionType_), strategyCallback(std::move(callback_)),
              timestamp(std::chrono::system_clock::now()), uuid(UUID())
    {}

    virtual ~Order() = default;

    virtual float maxOrderCost() = 0;

public:
    std::string security;
    uint32_t numShares;
    PositionType positionType;
    StrategyCallbackDef strategyCallback;
    std::chrono::system_clock::time_point timestamp;
    UUID uuid;
};

struct MarketOrder : public Order
{
    MarketOrder(std::string security, uint32_t numShares, PositionType positionType, float marketPrice_, StrategyCallbackDef callback_)
            : Order(std::move(security), numShares, positionType, std::move(callback_)), marketPrice(marketPrice_)
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
            : Order(std::move(security), numShares, positionType, std::move(callback_)), limitPrice(limitPrice_)
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
