#include "./SingletonFactory.h"
namespace ReiToEngine {
RTENGINE_API MemoryManager& GetMemoryManager()
{
    return SingletonFactory::GetMemoryManager();
}

std::once_flag SingletonFactory::once_flag_set;
std::once_flag SingletonFactory::once_flag_default;
}  // namespace ReiToEngine


