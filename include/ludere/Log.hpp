//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_LOG_HPP
#define LUDERE_LOG_HPP


#include <cstdarg>
#include <iostream>

namespace lud {

class Log
{
public:
    static inline void debug_log(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        printf("\033[0;32mLudere Debug:\033[0m %s\n", fmt);
    }

    static inline void warn_log(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        printf("\033[0;33mLudere Warning:\033[0m %s\n", fmt);
    }

    static inline void error_log(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        printf("\033[0;31mLudere Error:\033[0m %s\n", fmt);
    }
};

}

#define LD_DEBUG(fmt, ...) Log::debug_log(fmt, __VA_ARGS__)
#define LD_WARN(fmt, ...) Log::warn_log(fmt, __VA_ARGS__)
#define LD_ERROR(fmt, ...) Log::error_log(fmt, __VA_ARGS__)


#endif //LUDERE_LOG_HPP
