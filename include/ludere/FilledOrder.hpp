//
// Created by Ryan Elliott on 8/8/20.
//

#ifndef SUCCESSOR_FILLEDORDER_HPP
#define SUCCESSOR_FILLEDORDER_HPP


#include <chrono>
#include <cstdint>
#include <string>

#include <ludere/UUID.hpp>

namespace lud {

struct FilledOrder
{
    enum class FilledOrderStatus
    {
        kSuccess,
        kInsufficientFunds,
        kExpired
    };

    FilledOrder(std::string security_, uint32_t numShares_, float sharePrice_, FilledOrderStatus orderStatus_,
                UUID uuid_)
            : security(std::move(security_)), numShares(numShares_), sharePrice(sharePrice_), orderStatus(orderStatus_),
              uuid(uuid_), timestamp(std::chrono::system_clock::now())
    {}

    [[nodiscard]] float totalOrderCost() const
    {
        return numShares * sharePrice;
    }

    static std::unique_ptr<FilledOrder> generateFailureFilledOrder(FilledOrderStatus orderStatus, UUID uuid_)
    {
        return std::move(std::make_unique<FilledOrder>(nullptr, 0, 0, orderStatus, uuid_));
    }

    std::string security;
    uint32_t numShares;
    float sharePrice;
    FilledOrderStatus orderStatus;
    std::chrono::system_clock::time_point timestamp;
    UUID uuid;
};

}


#endif //SUCCESSOR_FILLEDORDER_HPP
