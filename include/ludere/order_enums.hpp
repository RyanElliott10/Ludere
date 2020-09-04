//
// Created by Ryan Elliott on 8/24/20.
//

#ifndef SUCCESSOR_ORDER_ENUMS_HPP
#define SUCCESSOR_ORDER_ENUMS_HPP


namespace lud::enums::order {

enum class position_types
{
  LONG,
  SHORT
};
enum class types
{
  LIMIT,
  MARKET
};
enum class signals
{
  BUY,
  SELL
};

enum class lifetime_durations
{
  HOUR = 3600,
  DAY = 3600 * 24,
  WEEK = 3600 * 24 * 7
};
enum class fill_statuses
{
  SUCCESS,
  INSUFFICIENT_FUNDS,
  EXPIRED
};

}

#endif //SUCCESSOR_ORDER_ENUMS_HPP
