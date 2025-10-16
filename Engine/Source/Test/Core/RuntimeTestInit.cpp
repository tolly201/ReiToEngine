#include "L20_Platform/L23_SingletonFactory/SingletonFactory.h"
#include "L20_Platform/L21_Logger/Logger.h"

// 提供 SingletonFactory::memoryManager 的定义，避免引用外部 Runtime 的实例
namespace ReiToEngine {
    MemoryManager SingletonFactory::memoryManager{};
}

// 可选：初始化日志系统桩（如果声明存在）
bool InitializeLog();
void TerminateLog();

#ifndef NDEBUG
static struct _RT_Test_RuntimeBootstrap {
    _RT_Test_RuntimeBootstrap() {
        // 可在此做一些最小的初始化，如日志
        // if (InitializeLog) InitializeLog();
        RT_LOG_INFO_PLATFORM("[TestBootstrap] Minimal runtime bound for tests.");
    }
    ~_RT_Test_RuntimeBootstrap() {
        // if (TerminateLog) TerminateLog();
    }
} _rt_test_runtime_bootstrap;
#endif
