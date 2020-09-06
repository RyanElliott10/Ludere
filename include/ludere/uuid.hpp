//
// Created by Ryan Elliott on 7/31/20.
//

#ifndef LUDERE_UUID_HPP
#define LUDERE_UUID_HPP


#include <memory>

namespace lud {

using uuid_hash = uint64_t;

class uuid
{
public:
    uuid() : m_id(uuid::s_inc++)
    {}

    ~uuid() = default;

    [[nodiscard]] __attribute__((always_inline))
    uuid_hash hash() const
    {
        return m_id;
    }

    __attribute__((always_inline))
    bool operator==(const uuid &other_) const
    {
        return m_id == other_.m_id;
    }

public:
    uint64_t m_id;

private:
    static inline uint64_t s_inc {0};
};

}


#endif //LUDERE_UUID_HPP
