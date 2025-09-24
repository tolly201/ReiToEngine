#ifndef PLATFORM_LOGGER_LOGGER_H
#define PLATFORM_LOGGER_LOGGER_H
#include "../L21_Console/Functions.h"
#include "../L22_Timer/Functions.h"
#include <iostream>
#include <cstdarg>
#include <sstream>
#include <utility>
#include <format>
#define LOG_FATAL_ENABLED 1
#define LOG_ERROR_ENABLED 1
namespace{
const char* level_strings[6]
{
    "[FATAL]: ",
    "[ERROR]: ",
    "[WARN]: ",
    "[INFO]: ",
    "[DEBUG]: ",
    "[TRACE]: ",
};
}

#if RT_OHI_DEBUG == 1
    #define LOG_WARN_ENABLED 1
    #define LOG_INFO_ENABLED 1
    #define LOG_DEBUG_ENABLED 1
    #define LOG_TRACE_ENABLED 1
#else
    #define LOG_WARN_ENABLED 0
    #define LOG_INFO_ENABLED 0
    #define LOG_DEBUG_ENABLED 0
    #define LOG_TRACE_ENABLED 0
#endif

enum E_LOG_LEVEL : u8
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
};

bool InitializeLog();
void TerminateLog();

// 支持 0 或 多参数的通用日志函数；避免空参数导致可变参数宏需要 GNU ##__VA_ARGS__ 扩展
template <typename... Args>
void Log(E_LOG_LEVEL level, Args&&... args)
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
}

template <typename... Args>
void LogFmt(E_LOG_LEVEL level, std::format_string<Args...> fmt, Args&&... args)
{
    std::stringstream ss;
    ss << level_strings[level];
    ss << std::format(fmt, std::forward<Args>(args)...);
    ss << std::endl;

    if (level < LOG_LEVEL_WARN)
        RT_Platform_ConsoleWriteError(ss.str().c_str(), level);
    else
        RT_Platform_ConsoleWrite(ss.str().c_str(), level);
}

// 日志宏定义（根据日志级别控制是否启用）
// 直接使用 C++20 __VA_OPT__ 以支持空参数形式： RT_LOG_INFO();
// 要求工程以 C++20 编译；这样可避免 GNU ##__VA_ARGS__ 产生的警告。
#if LOG_FATAL_ENABLED == 1
    #define RT_LOG_FATAL(...)          Log(E_LOG_LEVEL::LOG_LEVEL_FATAL __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_FATAL_FMT(fmt, ...) LogFmt(E_LOG_LEVEL::LOG_LEVEL_FATAL, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_FATAL(...)
    #define RT_LOG_FATAL_FMT(fmt, ...)
#endif

#if LOG_ERROR_ENABLED == 1
    #define RT_LOG_ERROR(...)          Log(E_LOG_LEVEL::LOG_LEVEL_ERROR __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_ERROR_FMT(fmt, ...) LogFmt(E_LOG_LEVEL::LOG_LEVEL_ERROR, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_ERROR(...)
    #define RT_LOG_ERROR_FMT(fmt, ...)
#endif

#if LOG_WARN_ENABLED == 1
    #define RT_LOG_WARN(...)           Log(E_LOG_LEVEL::LOG_LEVEL_WARN __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_WARN_FMT(fmt, ...)  LogFmt(E_LOG_LEVEL::LOG_LEVEL_WARN, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_WARN(...)
    #define RT_LOG_WARN_FMT(fmt, ...)
#endif

#if LOG_INFO_ENABLED == 1
    #define RT_LOG_INFO(...)           Log(E_LOG_LEVEL::LOG_LEVEL_INFO __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_INFO_FMT(fmt, ...)  LogFmt(E_LOG_LEVEL::LOG_LEVEL_INFO, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_INFO(...)
    #define RT_LOG_INFO_FMT(fmt, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
    #define RT_LOG_DEBUG(...)          Log(E_LOG_LEVEL::LOG_LEVEL_DEBUG __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_DEBUG_FMT(fmt, ...) LogFmt(E_LOG_LEVEL::LOG_LEVEL_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_DEBUG(...)
    #define RT_LOG_DEBUG_FMT(fmt, ...)
#endif

#if LOG_TRACE_ENABLED == 1
    #define RT_LOG_TRACE(...)          Log(E_LOG_LEVEL::LOG_LEVEL_TRACE __VA_OPT__(,) __VA_ARGS__)
    #define RT_LOG_TRACE_FMT(fmt, ...) LogFmt(E_LOG_LEVEL::LOG_LEVEL_TRACE, fmt __VA_OPT__(,) __VA_ARGS__)
#else
    #define RT_LOG_TRACE(...)
    #define RT_LOG_TRACE_FMT(fmt, ...)
#endif

#endif
