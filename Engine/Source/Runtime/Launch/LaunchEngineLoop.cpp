#include "Platform/Application/Include/RTMacApplication.h"

#ifdef RT_SYSTEM_WINDOWS

#elif defined(RT_SYSTEM_APPLE) // macOS

ReiToEngine::RTMacApplication ReiToEngine::RTMacApplication::instance;

ReiToEngine::RTApplication* ReiToEngine::RTApplication::instance_ptr = &ReiToEngine::RTMacApplication::instance;
#elif defined(RT_SYSTEM_LINUX) // Linux

#else
#endif
