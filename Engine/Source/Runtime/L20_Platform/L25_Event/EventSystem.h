#ifndef PLATFORM_EVENT_SYSTEM_H
#define PLATFORM_EVENT_SYSTEM_H
#include "L0_Macro/Include.h"
#include "L20_Platform/L23_SingletonFactory/RuntimeSingleton.h"
#include "L20_Platform/L21_Logger/Logger.h"
#include "Event.h"
#include "EventEnums.h"
namespace ReiToEngine
{
class RTENGINE_API EventSystem : public Runtime_Singleton<EventSystem>
{
public:
    EventSystem() = default;
    ~EventSystem() = default;

    b8 Initialize() override;
    b8 Tick(f64) override;
    b8 Terminate() override;

    b8 RegisterEvent(u32 code, void* listener, PFN_on_event callback);
    b8 UnregisterEvent(u32 code, void* listener, PFN_on_event callback);
    b8 TriggerEvent(u32 code, void* sender, event_context context);
private:
    b8 is_initialized = false;
    event_system_state state;
};
}
#endif
