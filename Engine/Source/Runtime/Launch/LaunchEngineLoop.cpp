#include "LaunchEngineLoop.h"
#include "Core/Macro/Macro.h"
#include "Platform/Application/Include/RTApplication.h"
#include "Platform/Enums/RTApplicationEnums.h"

#ifdef RT_SYSTEM_WINDOWS
#include "Platform/Application/Include/RTWindowsApplication.h"
ReiToEngine::RTWindowsApplication ReiToEngine::RTWindowsApplication::instance;

ReiToEngine::RTApplication* ReiToEngine::RTApplication::instance_ptr = &ReiToEngine::RTWindowsApplication::instance;

#elif defined(RT_SYSTEM_APPLE) // macOS
#include "Platform/Application/Include/RTMacApplication.h"

ReiToEngine::RTMacApplication ReiToEngine::RTMacApplication::instance;

ReiToEngine::RTApplication* ReiToEngine::RTApplication::instance_ptr = &ReiToEngine::RTMacApplication::instance;

#elif defined(RT_SYSTEM_LINUX) // Linux

#else
#endif

int RuntimeMainLoopEntry(int argc, const char* argv[])
{
    RTFATAL("test");
    RTERROR("test");
    RTDEBUG("test");
    RTTRACE("test");
    RTINFO("test");
    RTWARN("test");
    // RTASSERT(0 == 0);
    ReiToEngine::ApplicatonConfig app_config;
    app_config.name = "";
    app_config.start_height = 1024;
    app_config.start_width = 768;
    app_config.start_pos_y = 100;
    app_config.start_pos_x = 100;

    ReiToEngine::RTApplication::Instance().Initialize(app_config);
    printf("init\n");
    ReiToEngine::RTApplication::Instance().Run();
    printf("run \n");
    while (ReiToEngine::RTApplication::Instance().GetConstApplicationState().is_running) {
        while (!ReiToEngine::RTApplication::Instance().GetConstApplicationState().is_paused) {
            ReiToEngine::RTApplication::Instance().Tick();
        }
    }

    printf("terminate\n");
    ReiToEngine::RTApplication::Instance().Terminate();
    return 0;
}
