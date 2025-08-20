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
    void RTCreateWindow()
    {
        u32 index = windowsManager_ptr->RTCreateWindow(400,400, 100, 100, 4);
        PlatformInputMonitor* monitor = input_system_ptr->CreateOrGetMonitor(windowsManager_ptr->GetWindow(index));
        windowsManager_ptr->GetWindow(index)->SetInputMonitor(monitor);
    }

    protected:
    static RTApplication instance_raw;
    ApplicationState app_state;
    static RTApplication* instance_ptr;
    EventSystem* event_system_ptr;
    InputSystem* input_system_ptr;

    // RenderManager* renderManager_ptr;
    WindowsManager* windowsManager_ptr;
    b8 initialized = false;
    IGame* game_instance;
    private:
    void CreateMainWindow()
    {
        app_state.main_window.window_ptr = windowsManager_ptr->GetWindow(        windowsManager_ptr->RTCreateWindow(app_state.width, app_state.height, app_state.pos_x, app_state.pos_y, 4));

        PlatformInputMonitor* monitor = input_system_ptr->CreateOrGetMonitor(app_state.main_window.window_ptr);
        app_state.main_window.window_ptr->SetInputMonitor(monitor);
    }
};
}
#endif
