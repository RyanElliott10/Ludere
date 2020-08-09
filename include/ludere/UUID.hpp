//
// Created by Ryan Elliott on 7/31/20.
//

#ifndef LUDERE_UUID_HPP
#define LUDERE_UUID_HPP


#include <memory>

namespace lud {

using UUIDHash = uint64_t;

class UUID
{
public:
    UUID() : m_id(UUID::s_inc++)
    {}

    ~UUID() = default;

    [[nodiscard]] UUIDHash hash() const
    {
        return m_id;
    }

    inline bool operator==(const UUID &other) const
    {
        return m_id == other.m_id;
    }

public:
    uint64_t m_id;

private:
    static inline uint64_t s_inc = 0;
};

}


#endif //LUDERE_UUID_HPP
