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

namespace lud {

class OrderUUID
{
public:
    OrderUUID() : m_id(OrderUUID::s_inc++)
    {}

    ~OrderUUID() = default;

private:
    uint64_t m_id;

    static inline uint64_t s_inc = 0;
};

struct Order
{
    enum class PositionType
    {
        kLongPosition,
        kShortPosition
    };

    Order(std::string security_, uint32_t numShares_, PositionType positionType_)
            : security(security_), numShares(numShares_), positionType(positionType_),
              timestamp(std::chrono::system_clock::now()), uuid(OrderUUID())
    {}

    virtual ~Order() = default;

    virtual float maxOrderCost() = 0;

public:
    std::string security;
    uint32_t numShares;
    PositionType positionType;
    std::chrono::system_clock::time_point timestamp;
    OrderUUID uuid;
};

struct MarketOrder : public Order
{
    MarketOrder(std::string security, uint32_t numShares, PositionType positionType, float marketPrice_)
            : Order(security, numShares, positionType), marketPrice(marketPrice_)
    {}

    float maxOrderCost() override
    {
        return marketPrice * numShares;
    }

    float marketPrice;
};

struct LimitOrder : public Order
{
    LimitOrder(std::string security, uint32_t numShares, PositionType positionType, float limitPrice_)
            : Order(security, numShares, positionType), limitPrice(limitPrice_)
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
