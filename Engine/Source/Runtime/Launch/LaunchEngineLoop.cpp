#include "Core/Macro/Macro.h"

#ifdef RT_SYSTEM_WINDOWS

#elif defined(RT_SYSTEM_APPLE) // macOS
#include "Platform/Application/Include/RTMacApplication.h"

ReiToEngine::RTMacApplication ReiToEngine::RTMacApplication::instance;

ReiToEngine::RTApplication* ReiToEngine::RTApplication::instance_ptr = &ReiToEngine::RTMacApplication::instance;

#elif defined(RT_SYSTEM_LINUX) // Linux


#else
#endif

int RuntimeMainLoopEntry(int argc, const char* argv[])
{
    ReiToEngine::RTApplication::Instance().Initialize();
    // ReiToEngine::RTApplication::Instance().Run();
    // ReiToEngine::RTApplication::Instance().Tick();
    return 0;
}
