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
void Log(E_LOG_LEVEL level, const char* message, Args&&... args)
{
    std::stringstream ss;
    ss << level_strings[level] << message << std::endl;

    ((ss << std::forward<Args>(args) << std::endl), ...);

    if (level < LOG_LEVEL_WARN)
    {
        printf("level: %d\n", level);
        RT_Platform_ConsoleWriteError(ss.str().c_str(), level);
    }
    else
    {
        printf("level: %d\n", level);
        RT_Platform_ConsoleWrite(ss.str().c_str(), level);
    }
}

// 日志宏定义（根据日志级别控制是否启用）
#if LOG_FATAL_ENABLED == 1
    #define RT_LOG_FATAL(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#else
    #define RT_LOG_FATAL(message, ...)
#endif

#if LOG_ERROR_ENABLED == 1
    #define RT_LOG_ERROR(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#else
    #define RT_LOG_ERROR(message, ...)
#endif

#if LOG_WARN_ENABLED == 1
    #define RT_LOG_WARN(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
    #define RT_LOG_WARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
    #define RT_LOG_INFO(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
    #define RT_LOG_INFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
    #define RT_LOG_DEBUG(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
    #define RT_LOG_DEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
    #define RT_LOG_TRACE(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
    #define RT_LOG_TRACE(message, ...)
#endif

#endif
