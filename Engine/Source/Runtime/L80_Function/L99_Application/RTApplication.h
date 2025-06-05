#ifndef CORE_APPLICATION_RTAPPLICATION_H
#define CORE_APPLICATION_RTAPPLICATION_H
#include "L20_Platform/Include.h"
#include "RTApplicationEnums.h"
#include "L80_function/L99_Game/Game.h"
namespace ReiToEngine{
class RTENGINE_API RTApplication : public SingletonFactory
{
    friend class SingletonFactory;
public:
    static RTApplication& Instance();
    RTApplication();
    ~RTApplication();
    virtual b8 Initialize(IGame*);
    virtual b8 StartGame();
    virtual b8 Run();
    virtual void Terminate();
    const ApplicationState& GetConstApplicationState();
    ApplicationState& GetApplicationState();

    protected:
    static RTApplication instance_raw;
    ApplicationState app_state;
    static RTApplication* instance_ptr;
    EventSystem* event_system_ptr;
    InputMonitor* input_system_ptr;

    // RenderManager* renderManager_ptr;
    // WindowsManager* windowsManager_ptr;

    b8 initialized = false;
    IGame* game_instance;
};
}
#endif
