// Engine/Source/Runtime/Core/HAL/Input/Include/IInputMonitor.h
#ifndef PLATFORM_WINDOWS_INPUT_MONITOR_H
#define PLATFORM_WINDOWS_INPUT_MONITOR_H
#ifdef RT_SYSTEM_WINDOWS
#include "InputMonitor.h"
namespace ReiToEngine {

class RTENGINE_API WindowsInputMonitor : public InputMonitor{
public:
    WindowsInputMonitor();
    ~WindowsInputMonitor();
    ProcessNativeEvent(MSG* msg);
};

}  // namespace ReiToEngine
#endif
#endif
