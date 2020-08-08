//
// Created by Ryan Elliott on 7/30/20.
//

#ifndef LUDERE_LOG_HPP
#define LUDERE_LOG_HPP


#include <cstdarg>
#include <cstdio>

// Xcode doesn't work well with ANSI color-coded output
#define MAGENTA_ANSI_CODE "\033[01;35m"
#define YELLOW_ANSI_CODE "\033[01;33m"
#define RED_ANSI_CODE "\033[01;31m"

namespace ludere {

class Log
{
public:
    static inline void debug(const char *format, ...)
    {
        printf("%s%s", MAGENTA_ANSI_CODE, Log::s_debugMsg);
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        printf("\033[0m\n");
    }

    static inline void warn(const char *format, ...)
    {
        printf("%s%s", YELLOW_ANSI_CODE, Log::s_warnMsg);
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        printf("\n");
    }

    static inline void error(const char *format, ...)
    {
        printf("%s%s", RED_ANSI_CODE, Log::s_errorMsg);
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
        printf("\n");
    }

private:
    static constexpr const char *s_debugMsg = "LUD_DEBUG: ";
    static constexpr const char *s_warnMsg = "LUD_WARN: ";
    static constexpr const char *s_errorMsg = "LUD_ERROR: ";
};

#define LD_DEBUG(format, ...)\
    ludere::Log::debug(format, __VA_ARGS__);
#define LD_WARN(format, ...)\
    ludere::Log::warn(format, __VA_ARGS__);
#define LD_ERROR(format, ...)\
    ludere::Log::error(format, __VA_ARGS__);

}


#endif //LUDERE_LOG_HPP
