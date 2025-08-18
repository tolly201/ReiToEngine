#ifndef CORE_HAL_INPUT_SYSTEM_H
#define CORE_HAL_INPUT_SYSTEM_H
#include "L0_Macro/Include.h"
#include "InputEnums.h"
#include "L20_Platform/L31_SingletonFactory/RuntimeSingleton.h"
#include "InputMonitor.h"
#include "L20_Platform/L32_Containers/Map.h"
#ifdef RT_SYSTEM_WINDOWS
    #include "WindowsInputMonitor.h"
    using PlatformInputMonitor = ReiToEngine::WindowsInputMonitor;
#elif defined(RT_SYSTEM_APPLE)
    #include "MacOSXInputMonitor.h"
    using PlatformInputMonitor = ReiToEngine::MacOSXInputMonitor;
#elif RT_SYSTEM_LINUX
    #include "LinuxInputMonitor.h"
    using PlatformInputMonitor = LinuxInputMonitor;
#endif

namespace ReiToEngine {

using NativeHandle = void*;

class RTENGINE_API InputSystem : public Runtime_Singleton<InputSystem> {
friend class Runtime_Singleton<InputSystem>;
public:
    InputSystem();
    ~InputSystem();

    b8 Initialize();
    b8 Terminate();
    b8 Tick(f64);

    b8 CreateOrGetMonitor(NativeHandle handle);
    b8 DestroyMonitor(NativeHandle handle);

   private:
    b8 initialized = false;

    ReiToEngine::map<NativeHandle, PlatformInputMonitor*> input_monitors;

    PlatformInputMonitor* global_input_monitor = nullptr;
};

}  // namespace ReiToEngine
#endif
