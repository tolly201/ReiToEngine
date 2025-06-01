#ifndef PLATFORM_SINGLETON_SYSTEMS_EVENT_SYSTEM_H
#define PLATFORM_SINGLETON_SYSTEMS_EVENT_SYSTEM_H
#include "Core/MinimalCore.h"
#include "../../Singleton/RuntimeSingleton.h"
#include "../../Event/Event.h"
namespace ReiToEngine
{
class RTENGINE_API EventManager : public Runtime_Singleton<EventManager>
{
public:
    EventManager() = default;
    ~EventManager() = default;

    b8 Initialize() override;
    b8 Tick() override;
    b8 Terminate() override;

    b8 RegisterEvent(u32 code, void* listener, PFN_on_event callback);
    b8 UnregisterEvent(u32 code, void* listener, PFN_on_event callback);
    b8 TriggerEvent(u32 code, void* sender, event_context context);
private:
    event_system_state state;
    b8 is_initialized = false;
};
}
#endif
