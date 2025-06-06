#ifndef PLATFORM_LOGGER_LOGGER_H
#define PLATFORM_LOGGER_LOGGER_H
#include "../L21_Console/Functions.h"
#include "../L22_Timer/Functions.h"
#include <iostream>
#include <cstdarg>
#include <sstream>
#include <utility>
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

template <typename... Args>
void Log(E_LOG_LEVEL level, Args&&... args)
{
    std::stringstream ss;
    ss << level_strings[level];
    ((ss << std::forward<Args>(args) << std::endl), ...);

    if (level < LOG_LEVEL_WARN)
    {
        RT_Platform_ConsoleWriteError(ss.str().c_str(), level);
    }
    else
    {
        RT_Platform_ConsoleWrite(ss.str().c_str(), level);
    }
}

// 日志宏定义（根据日志级别控制是否启用）
#if LOG_FATAL_ENABLED == 1
    #define RT_LOG_FATAL(...) Log(E_LOG_LEVEL::LOG_LEVEL_FATAL, ##__VA_ARGS__)
#else
    #define RT_LOG_FATAL(...)
#endif

#if LOG_ERROR_ENABLED == 1
    #define RT_LOG_ERROR(...) Log(E_LOG_LEVEL::LOG_LEVEL_ERROR, ##__VA_ARGS__)
#else
    #define RT_LOG_ERROR(...)
#endif

#if LOG_WARN_ENABLED == 1
    #define RT_LOG_WARN(...) Log(E_LOG_LEVEL::LOG_LEVEL_WARN, ##__VA_ARGS__)
#else
    #define RT_LOG_WARN(...)
#endif

#if LOG_INFO_ENABLED == 1
    #define RT_LOG_INFO(...) Log(E_LOG_LEVEL::LOG_LEVEL_INFO, ##__VA_ARGS__)
#else
    #define RT_LOG_INFO(...)
#endif

#if LOG_DEBUG_ENABLED == 1
    #define RT_LOG_DEBUG(...) Log(E_LOG_LEVEL::LOG_LEVEL_DEBUG, ##__VA_ARGS__)
#else
    #define RT_LOG_DEBUG(...)
#endif

#if LOG_TRACE_ENABLED == 1
    #define RT_LOG_TRACE(...) Log(E_LOG_LEVEL::LOG_LEVEL_TRACE, ##__VA_ARGS__)
#else
    #define RT_LOG_TRACE(...)
#endif

#endif
