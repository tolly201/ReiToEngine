// Engine/Source/Runtime/Core/HAL/Input/Include/IInputMonitor.h
#ifndef PLATFORM_WINDOWS_INPUT_MONITOR_H
#define PLATFORM_WINDOWS_INPUT_MONITOR_H
#include "InputMonitor.h"
namespace ReiToEngine {

class RTENGINE_API WindowsInputMonitor : public InputMonitor{
public:
    WindowsInputMonitor();
    ~WindowsInputMonitor();

};

}  // namespace ReiToEngine
#endif
