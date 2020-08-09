//
// Created by Ryan Elliott on 8/8/20.
//

#ifndef SUCCESSOR_FILLEDORDER_HPP
#define SUCCESSOR_FILLEDORDER_HPP


#include <chrono>
#include <cstdint>
#include <string>

namespace lud {

struct FilledOrder
{
    enum class FilledOrderStatus
    {
        kSuccess,
        kInsufficientFunds
    };

    FilledOrder(std::string security_, uint32_t numShares_, float sharePrice_, FilledOrderStatus orderStatus_)
            : security(std::move(security_)), numShares(numShares_), sharePrice(sharePrice_), orderStatus(orderStatus_),
              timestamp(std::chrono::system_clock::now())
    {}

    [[nodiscard]] float totalOrderCost() const {
        return numShares * sharePrice;
    }

    std::string security;
    uint32_t numShares;
    float sharePrice;
    FilledOrderStatus orderStatus;
    std::chrono::system_clock::time_point timestamp;
};

}


#endif //SUCCESSOR_FILLEDORDER_HPP
