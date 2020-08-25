//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_LOG_HPP
#define LUDERE_LOG_HPP


#include <cstdarg>
#include <cstdio>

#define MAGENTA_ANSI_CODE "\033[01;35m"
#define YELLOW_ANSI_CODE "\033[01;33m"
#define RED_ANSI_CODE "\033[01;31m"

namespace lud {

class log
{
public:
    static inline void debug(const char *format, ...)
    {
        printf("%s%s", MAGENTA_ANSI_CODE, log::s_debug_msg);
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        printf("\033[0m\n");
    }

    static inline void warn(const char *format, ...)
    {
        printf("%s%s", YELLOW_ANSI_CODE, log::s_warn_msg);
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        printf("\033[0m\n");
    }

    static inline void error(const char *format, ...)
    {
        printf("%s%s", RED_ANSI_CODE, log::s_error_msg);
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        printf("\033[0m\n");
    }

private:
    static constexpr const char *s_debug_msg = "LUD_DEBUG: ";
    static constexpr const char *s_warn_msg = "LUD_WARN: ";
    static constexpr const char *s_error_msg = "LUD_ERROR: ";
};

#ifndef NDEBUG
#define LUD_DEBUG(format, ...)\
    lud::log::debug(format, __VA_ARGS__);
#define LUD_WARN(format, ...)\
    lud::log::warn(format, __VA_ARGS__);
#define LUD_ERROR(format, ...)\
    lud::log::error(format, __VA_ARGS__);
#else
#define LUD_DEBUG(format, ...)
#define LUD_WARN(format, ...)
#define LUD_ERROR(format, ...)
#endif

}


#endif //LUDERE_LOG_HPP
