//
// Created by Ryan Elliott on 8/14/20.
//

#ifndef SUCCESSOR_DATASTREAMABLE_HPP
#define SUCCESSOR_DATASTREAMABLE_HPP


#include <unordered_map>

#include <ludere/candlestick_data.hpp>

namespace lud {

struct candlestick_data_aggregate
{
    std::unordered_map<std::string, candlestick_data> m_map;
    time_t m_timestamp;

    template<typename T>
    __attribute__((always_inline))
    candlestick_data at(T key) const
    { return m_map.at(key); }

    template<typename T>
    __attribute__((always_inline))
    auto find(T key) const
    { return m_map.find(key); }

    [[nodiscard]] __attribute__((always_inline))
    auto end() const
    { return m_map.end(); }

    [[nodiscard]] __attribute__((always_inline))
    bool empty() const
    { return m_map.empty(); }

    template<typename T, typename V>
    __attribute__((always_inline))
    void emplace(T key, V value)
    { m_map.emplace(key, value); }
};

struct datastreamable
{
    virtual ~datastreamable() = default;

    /// Called by an exchange to request the next data points.
    virtual candlestick_data_aggregate poll_next_stream() = 0;
};

}


#endif //SUCCESSOR_DATASTREAMABLE_HPP
