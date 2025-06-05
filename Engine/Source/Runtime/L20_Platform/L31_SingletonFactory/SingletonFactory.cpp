#include "./SingletonFactory.h"
namespace ReiToEngine {
RT_FORCEINLINE MemoryManager& GetMemoryManager()
{
    return SingletonFactory::GetMemoryManager();
}
}  // namespace ReiToEngine


