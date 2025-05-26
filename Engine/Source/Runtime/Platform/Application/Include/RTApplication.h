#ifndef CORE_APPLICATION_RTAPPLICATION_H
#define CORE_APPLICATION_RTAPPLICATION_H
#include "../../Enums/RTApplicationEnums.h"
#include "Platform/Singleton/SingletonFactory.h"
#include "Platform/WindowsManager/Include/WindowsManager.h"
#include "Platform/WindowsManager/WindowsManager.h"
#include "Platform/InputSystem/Include/InputSystem.h"

#include "Function/RenderManager/Include/RenderManager.h"
#include "Core/MinimalCore.h"
#include "Launch/GameTypes.h"
namespace ReiToEngine{
class RTENGINE_API RTApplication : public SingletonFactory
{
    friend class SingletonFactory;
public:
    RTDebugHeapMemoryManager& GetMemoryManager();
    static RTApplication& Instance();
    RTApplication();
    ~RTApplication();
    virtual b8 Initialize(Game&);
    virtual b8 StartGame();
    virtual b8 Run();
    virtual void Terminate();
    const ApplicationState& GetConstApplicationState();
    ApplicationState& GetApplicationState();

    protected:
    ApplicationState app_state;
    static RTApplication* instance_ptr;
    WindowsManager* windowsManager_ptr;
    InputSystem* inputSystem_ptr;
    RenderManager* renderManager_ptr;

    b8 initialized = false;
    Game game_instance;
};
}
#endif
