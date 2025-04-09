#include "Core/Macro/Macro.h"
#include "Platform/Application/Include/RTApplication.h"

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
    printf("init\n");
    ReiToEngine::RTApplication::Instance().Run();
    printf("run \n");
    while (!ReiToEngine::RTApplication::Instance().shouldQuit) {
        ReiToEngine::RTApplication::Instance().Tick();
        printf("keep\n");
    }
    printf("terminate\n");
    ReiToEngine::RTApplication::Instance().Terminate();
    return 0;
}
