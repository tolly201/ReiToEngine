#ifndef CORE_PLATFORM_HAL_INPUT_MONITOR_H
#define CORE_PLATFORM_HAL_INPUT_MONITOR_H

#include "InputEnums.h"
namespace ReiToEngine {

class InputMonitor
{
public:
    InputMonitor() = default;
    virtual ~InputMonitor() = default;
    virtual void AddInputCallback(InputCallback callback)
    {
        inputCallback = std::move(callback);
    };
    virtual void Activate()
    {
        isActive = true;
    };
    virtual void InActivate()
    {
        isActive = false;
    };
protected:
    InputCallback inputCallback;
    bool isActive;
};

}
#endif
