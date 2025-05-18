#ifndef CORE_APPLICATION_RTAPPLICATION_H
#define CORE_APPLICATION_RTAPPLICATION_H
#include "Platform/Enums/RTApplicationEnums.h"
#include "Platform/Singleton/SingletonFactory.h"
#include "Platform/WindowsManager/Include/WindowsManager.h"
#include "Platform/WindowsManager/WindowsManager.h"
#include "Platform/InputSystem/Include/InputSystem.h"

#include "Function/RenderManager/Include/RenderManager.h"
#include "Core/MinimalCore.h"

namespace ReiToEngine{
class RTENGINE_API RTApplication : public SingletonFactory
{
    friend class SingletonFactory;
public:
    RTDebugHeapMemoryManager& GetMemoryManager();
    static RTApplication& Instance();
    RTApplication();
    ~RTApplication();
    virtual void Initialize(ApplicatonConfig&);
    virtual void Run();
    virtual void Tick();
    virtual void Terminate();
    const ApplicationState& GetConstApplicationState();
    ApplicationState& GetApplicationState();

    protected:
    ApplicationState app_state;
    static RTApplication* instance_ptr;
    WindowsManager* windowsManager_ptr;
    InputSystem* inputSystem_ptr;
    RenderManager* renderManager_ptr;

    ApplicatonConfig app_config;
};
}
#endif
