#include "./SingletonFactory.h"
namespace ReiToEngine {
MemoryManager& GetMemoryManager()
{
    return SingletonFactory::GetMemoryManager();
}
}  // namespace ReiToEngine


