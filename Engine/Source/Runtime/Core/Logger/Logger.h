#ifndef CORE_LOGGER_LOGGER_H
#define CORE_LOGGER_LOGGER_H
#include "../Macro/Functions.h"
#include "Core/Macro/Macro.h"
#include "Core/HAL/Console/Include/IConsole.h"
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
        RT_HAL_ConsoleWriteError(ss.str().c_str(), level);
    }
    else
    {
        printf("level: %d\n", level);
        RT_HAL_ConsoleWrite(ss.str().c_str(), level);
    }
}

// 日志宏定义（根据日志级别控制是否启用）
#if LOG_FATAL_ENABLED == 1
    #define RTFATAL(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_FATAL, message, ##__VA_ARGS__)
#else
    #define RTFATAL(message, ...)
#endif

#if LOG_ERROR_ENABLED == 1
    #define RTERROR(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#else
    #define RTERROR(message, ...)
#endif

#if LOG_WARN_ENABLED == 1
    #define RTWARN(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#else
    #define RTWARN(message, ...)
#endif

#if LOG_INFO_ENABLED == 1
    #define RTINFO(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#else
    #define RTINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
    #define RTDEBUG(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#else
    #define RTDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
    #define RTTRACE(message, ...) Log(E_LOG_LEVEL::LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#else
    #define RTTRACE(message, ...)
#endif

#endif
