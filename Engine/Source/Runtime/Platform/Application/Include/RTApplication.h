#ifndef CORE_APPLICATION_RTAPPLICATION_H
#define CORE_APPLICATION_RTAPPLICATION_H
#include <cstddef>
#include "Platform/HAL/System/Include/SystemInfo.h"
#include "Platform/Singleton/SingletonFactory.h"
#include "Platform/HAL/System/System.h"
#include "Platform/WindowsManager/Include/WindowsManager.h"
#include "Platform/WindowsManager/WindowsManager.h"
#include "Platform/InputSystem/Include/InputSystem.h"

namespace ReiToEngine{
class RTApplication : public SingletonFactory
{
    friend class SingletonFactory;
public:
    RTDebugHeapMemoryManager& GetMemoryManager();
    static RTApplication& Instance();
    RTApplication();
    ~RTApplication();
    virtual void Initialize();
    virtual void Run();
    virtual void Tick();
    virtual void Terminate();
    bool shouldQuit;
protected:
    static RTApplication* instance_ptr;
    SystemInfo* systemInfo_ptr;
    WindowsManager* windowsManager_ptr;
    InputSystem* inputSystem_ptr;
};
}
#endif
