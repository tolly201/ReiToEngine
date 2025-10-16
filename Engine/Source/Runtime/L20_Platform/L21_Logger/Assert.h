#ifndef PLATFORM_LOGGER_ASSERT_H
#define PLATFORM_LOGGER_ASSERT_H
#include "L0_Macro/Include.h"
#define RT_ASSERT_PLATFORMIONS_ENABLED 1

RTENGINE_API void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#if RT_ASSERT_PLATFORMIONS_ENABLED == 1
#if __MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define debugBreak() __asm__ volatile("int $3")
#else
#define debugBreak() ((void)0) // 如果无法定义，则为空实现
#endif

#define RT_ASSERT_PLATFORM(expr)                                     \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }

#define RT_ASSERT_MESSAGE_PLATFORM(expr, message)                         \
    {                                                                     \
        if (expr) {                                                       \
        } else {                                                          \
            report_assertion_failure(#expr, message, __FILE__, __LINE__); \
            debugBreak();                                                 \
        }                                                                 \
    }

#ifdef _DEBUG
#define RT_ASSERT_DEBUG_PLATFORM(expr)                                         \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }
#endif
#else
#define debugBreak()
#define RT_ASSERT_PLATFORM(expr)
#define RT_ASSERT_MESSAGE_PLATFORM(expr, message)
#define RT_ASSERT_DEBUG_PLATFORM(expr)
#endif

#endif
