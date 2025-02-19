#ifndef CORE_PLATFORM_PLATFORM_H
#define CORE_PLATFORM_PLATFORM_H 
#include <cstdint>
#include <cstddef>
namespace ReiToEngine
{
    #define RTENGINE_API __declspec(dllexport)
#ifdef _WIN32
    #define RT_SYSTEM_WINDOWS
    #ifdef _WIN64
        #define RT_SYSTEM_BIT_WIDTH 64
    #else
        #define RT_SYSTEM_BIT_WIDTH 32
    #endif
#elif defined(__APPLE__) // macOS
    #define RT_SYSTEM_APPLE
    #if defined(__x86_64__) || defined(__ppc64__)
        #define RT_SYSTEM_BIT_WIDTH 64
    #else
        #define RT_SYSTEM_BIT_WIDTH 32
    #endif
#elif defined(__linux__) // Linux
    #define RT_SYSTEM_LINUX
    #if defined(__x86_64__) || defined(__ppc64__)
        #define RT_SYSTEM_BIT_WIDTH 64
    #else
        #define RT_SYSTEM_BIT_WIDTH 32
    #endif
#else
    #define RT_SYSTEM_UNKNOWN
#endif

#ifdef RT_SYSTEM_WINDOWS
    #ifdef RT_EXPORTS // 定义这个宏表示正在编译 DLL
        #define RTENGINE_API __declspec(dllexport)
    #else // 使用 DLL 的项目
        #define RTENGINE_API __declspec(dllexport)
    #endif
    #define RT_FORCEINLINE __forceinline
#elif defined(RT_SYSTEM_APPLE) // macOS
    #define RTENGINE_API __attribute__((visibility("default")))
    #define RT_FORCEINLINE inline __attribute__((always_inline))
#elif defined(RT_SYSTEM_LINUX) // Linux
    #define RTENGINE_API __attribute__((visibility("default")))
    #define RT_FORCEINLINE inline __attribute__((always_inline))
#else
    #error "Unsupported platform"
#endif
}

#endif