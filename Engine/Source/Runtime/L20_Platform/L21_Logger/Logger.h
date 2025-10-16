#ifndef PLATFORM_LOGGER_LOGGER_H
#define PLATFORM_LOGGER_LOGGER_H
#include "../L21_Console/Functions.h"
#include "../L22_Timer/Functions.h"
#include <iostream>
#include <cstdarg>
#include <sstream>
#include <utility>
#include <format>

bool InitializeLog();
void TerminateLog();

// 支持 0 或 多参数的通用日志函数；避免空参数导致可变参数宏需要 GNU ##__VA_ARGS__ 扩展
template <typename... Args>
std::string Log(E_LOG_LEVEL level, Args&&... args)
{
    std::stringstream ss;
    ss << level_strings[level];
    if constexpr (sizeof...(Args) > 0)
    {
        // 保持原先“每个参数一行”行为
        ((ss << std::forward<Args>(args) << std::endl), ...);
    }
    else
    {
        ss << std::endl;
    }

    if (level < LOG_LEVEL_WARN)
        RT_Platform_ConsoleWriteError(ss.str().c_str(), level);
    else
        RT_Platform_ConsoleWrite(ss.str().c_str(), level);
    return ss.str();
}

template <typename... Args>
std::string LogFmt(E_LOG_LEVEL level, std::format_string<Args...> fmt, Args&&... args)
{
    std::stringstream ss;
    ss << level_strings[level];
    ss << std::format(fmt, std::forward<Args>(args)...);
    ss << std::endl;

    if (level < LOG_LEVEL_WARN)
        RT_Platform_ConsoleWriteError(ss.str().c_str(), level);
    else
        RT_Platform_ConsoleWrite(ss.str().c_str(), level);
    return ss.str();
}

// 日志宏定义（根据日志级别控制是否启用）
// 直接使用 C++20 __VA_OPT__ 以支持空参数形式： RT_LOG_INFO_PLATFORM();
// 要求工程以 C++20 编译；这样可避免 GNU ##__VA_ARGS__ 产生的警告。
#if LOG_FATAL_ENABLED == 1
    #define RT_LOG_FATAL_PLATFORM(...)          Log(E_LOG_LEVEL::LOG_LEVEL_FATAL __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_FATAL_FMT_PLATFORM(fmt, ...) LogFmt(E_LOG_LEVEL::LOG_LEVEL_FATAL, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_FATAL_PLATFORM(...)
    #define RT_LOG_FATAL_FMT_PLATFORM(fmt, ...)
#endif

#if LOG_ERROR_ENABLED == 1
    #define RT_LOG_ERROR_PLATFORM(...)          Log(E_LOG_LEVEL::LOG_LEVEL_ERROR __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_ERROR_FMT_PLATFORM(fmt, ...) LogFmt(E_LOG_LEVEL::LOG_LEVEL_ERROR, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_ERROR_PLATFORM(...)
    #define RT_LOG_ERROR_FMT_PLATFORM(fmt, ...)
#endif

#if LOG_WARN_ENABLED == 1
    #define RT_LOG_WARN_PLATFORM(...)           Log(E_LOG_LEVEL::LOG_LEVEL_WARN __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_WARN_FMT_PLATFORM(fmt, ...)  LogFmt(E_LOG_LEVEL::LOG_LEVEL_WARN, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_WARN_PLATFORM(...)
    #define RT_LOG_WARN_FMT_PLATFORM(fmt, ...)
#endif

#if LOG_INFO_ENABLED == 1
    #define RT_LOG_INFO_PLATFORM(...)           Log(E_LOG_LEVEL::LOG_LEVEL_INFO __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_INFO_FMT_PLATFORM(fmt, ...)  LogFmt(E_LOG_LEVEL::LOG_LEVEL_INFO, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_INFO_PLATFORM(...)
    #define RT_LOG_INFO_FMT_PLATFORM(fmt, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
    #define RT_LOG_DEBUG_PLATFORM(...)          Log(E_LOG_LEVEL::LOG_LEVEL_DEBUG __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_DEBUG_FMT_PLATFORM(fmt, ...) LogFmt(E_LOG_LEVEL::LOG_LEVEL_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_DEBUG_PLATFORM(...)
    #define RT_LOG_DEBUG_FMT_PLATFORM(fmt, ...)
#endif

#if LOG_TRACE_ENABLED == 1
    #define RT_LOG_TRACE_PLATFORM(...)          Log(E_LOG_LEVEL::LOG_LEVEL_TRACE __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_TRACE_FMT_PLATFORM(fmt, ...) LogFmt(E_LOG_LEVEL::LOG_LEVEL_TRACE, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_TRACE_PLATFORM(...)
    #define RT_LOG_TRACE_FMT_PLATFORM(fmt, ...)
#endif

#endif
