#ifndef CORE_MACRO_MACRO_H
#define CORE_MACRO_MACRO_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define RT_SYSTEM_WINDOWS
#    ifndef _WIN64
#        error "64-bit is required on Windows!"
#    endif
#elif defined(__linux__) || defined(__gnu_linux__)
#    define RT_SYSTEM_LINUX 1
#    if defined(__ANDROID__)
#        define RT_SYSTEM_ANDROID 1
#    endif
#elif defined(__unix__)
#    define RT_SYSTEM_UNIX 1
#elif defined(_POSIX_VERSION)
#    define RT_SYSTEM_POSIX 1
#elif __APPLE__
#include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
        #define RT_SYSTEM_IOS 1
        #define RT_SYSTEM_IOS_SIMULATOR 1
    #elif TARGET_OS_IPHONE
        #define RT_SYSTEM_IOS 1
    #elif TARGET_OS_MAC
        #define RT_SYSTEM_APPLE
// HACK: Should probably be in the Vulkan Renderer lib, not here.
        #define VK_USE_PLATFORM_MACOS_MVK
    #else
        #error "Unknown Apple platform"
    #endif
#else
    #define RT_SYSTEM_UNKNOWN
    #error "Unknown platform!"
#endif

#ifdef RT_EXPORTS
#    ifdef _MSC_VER
#        define RTENGINE_API __declspec(dllexport)
#    elif defined(__MINGW32__) || defined(__MINGW64__)
#        define RTENGINE_API __attribute__((dllexport))
#    else
#        define RTENGINE_API __attribute__((visibility("default")))
#    endif
#else
#    ifdef _MSC_VER
#        define RTENGINE_API __declspec(dllimport)
#    elif defined(__MINGW32__) || defined(__MINGW64__)
#        define RTENGINE_API __attribute__((dllimport))
#    else
#        define RTENGINE_API
#    endif
#endif

#ifdef _MSC_VER
    #define RT_FORCEINLINE __forceinline
#else
    #define RT_FORCEINLINE inline __attribute__((always_inline))
#endif

#if _DEBUG
#    define RT_OHI_DEBUG 1
#    define RT_OHI_RELEASE 0
#else
#    define RT_OHI_RELEASE 1
#    define RT_OHI_DEBUG 0
#endif

// Deprecation
#if defined(__clang__) || defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)
/** @brief Mark something (i.e. a function) as deprecated. */
#    define RTDEPRECATED(message) __attribute__((deprecated(message)))
#elif defined(_MSC_VER)
/** @brief Mark something (i.e. a function) as deprecated. */
#    define RTDEPRECATED(message) __declspec(deprecated(message))
#else
#    error "Unsupported compiler - don't know how to define deprecations!"
#endif

#define RT_SYSTEM_BIT_WIDTH 64

static const double RT_COMPARE_PRECISION = 1.0e-7;

#endif
