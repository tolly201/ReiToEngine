#include "Memory.h"
namespace ReiToEngine
{
    MemoryManager& GetMemoryManager()
    {
        return RTApplication::Instance().GetMemoryManager();
    }
}

