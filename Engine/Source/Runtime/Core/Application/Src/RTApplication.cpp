#include "../Include/RTApplication.h"

namespace ReiToEngine{
    RTApplication::RTApplication() = default;
    RTApplication::~RTApplication() = default;
    RTDebugHeapMemoryManager& RTApplication::GetMemoryManager()
    {
        return RTApplication::memoryManager;
    }

    RTApplication& RTApplication::Instance()
    {
        return instance;
    }

    void RTApplication::Initialize()
    {
        GetSingletonManager();
    }
    void RTApplication::Run()
    {

    }
    void RTApplication::Tick()
    {

    }
}
