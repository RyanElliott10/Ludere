//
// Created by Ryan Elliott on 7/31/20.
//

#ifndef LUDERE_UUID_HPP
#define LUDERE_UUID_HPP


#include <memory>

namespace lud {

// Universally Unique Identifier
class UUID
{
public:
    static inline std::unique_ptr<UUID> generateUUID() { return std::unique_ptr<UUID>(new UUID()); }

private:
    UUID() {
        m_id = UUID::s_inc++;
    }
    uint64_t m_id;

    static inline uint64_t s_inc = 0;
};

}


#endif //LUDERE_UUID_HPP
