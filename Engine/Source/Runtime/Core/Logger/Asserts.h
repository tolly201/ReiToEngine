#ifndef CORE_LOGGER_ASSERTS_H
#define CORE_LOGGER_ASSERTS_H
#include "../Macro/Functions.h"
#define RT_ASSERTIONS_ENABLED 1

RTENGINE_API void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#if RT_ASSERTIONS_ENABLED == 1
#if __MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define debugBreak() __asm__ volatile("int $3")
#else
#define debugBreak() ((void)0) // 如果无法定义，则为空实现
#endif

#define RTASSERT(expr)                                               \
    {                                                                \
        if (expr) {                                                  \
        } else {                                                     \
            report_assertion_failure(#expr, "", __FILE__, __LINE__); \
            debugBreak();                                            \
        }                                                            \
    }

#define RTASSERT_MESSAGE(expr, message)                                   \
    {                                                                     \
        if (expr) {                                                       \
        } else {                                                          \
            report_assertion_failure(#expr, message, __FILE__, __LINE__); \
            debugBreak();                                                 \
        }                                                                 \
    }

#ifdef _DEBUG
#define RTASSERT_DEBUG(expr)                                         \
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
#define RTASSERT(expr)
#define RTASSERT_MESSAGE(expr, message)
#define RTASSERT_DEBUG(expr)
#endif

#endif
