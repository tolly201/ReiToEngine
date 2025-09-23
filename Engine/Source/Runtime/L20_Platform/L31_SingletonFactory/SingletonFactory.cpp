#include "./SingletonFactory.h"
namespace ReiToEngine {
RTENGINE_API MemoryManager& GetMemoryManager()
{
    return SingletonFactory::GetMemoryManager();
}
}  // namespace ReiToEngine


