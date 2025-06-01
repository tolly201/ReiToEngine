#ifndef CORE_PLATFORM_MEMORY_MEMORY_H
#define CORE_PLATFORM_MEMORY_MEMORY_H
#include "./Include/RTCMemoryManager.h"
#include "../Application/Include/RTApplication.h"
namespace ReiToEngine
{
    using MemoryManager = RTCMemoryManager;
    MemoryManager& GetMemoryManager();
}

#endif
