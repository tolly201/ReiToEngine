#include "../Include/InputSystem.h"

#include "Core/HAL/Input/Include/InputEnums.h"

#ifdef RT_SYSTEM_APPLE
    #include "Core/HAL/Input/Include/MacOSXInputMonitor.h"
#endif

namespace ReiToEngine {

void ReceiveEventFromMonitor(const InputEvent& event)
{
    // InputSystem::Instance().ProcessInputEvent(event);
}

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

// void InputSystem::AddInputCallback(EINPUT_EVENT_TYPE type, InputCallback callback)
// {
//     printf("add call back");
//     if (callbacks.find(type) == callbacks.end())
//     {
//         callbacks[type] = std::vector<InputCallback>();
//     }

//     callbacks[type].push_back(callback);
//     printf("add call back%hhu,%lu",type,callbacks[type].size());
// }
// void InputSystem::RemoveInputCallbacks(EINPUT_EVENT_TYPE type)
// {
//     // if (callbacks.find(type) != callbacks.end())
//     // {
//     //     callbacks[type].clear();
//     // }
// }

// b8 InputSystem::ProcessInputEvent(const InputEvent& event)
// {
//     // eventList.push_back(event);
// }

b8 InputSystem::Initialize()
{
    return true;    // #ifdef RT_SYSTEM_APPLE
    // m_eventMonitor = new MacOSXInputMonitor();
    // #endif

    // m_eventMonitor->AddInputCallback(ReceiveEventFromMonitor);
    // m_eventMonitor->Activate();
}

b8 InputSystem::Terminate()
{
    return true;
}

b8 InputSystem::Tick()
{
    return true;
    // for (auto& event : eventList)
    // {
    //     for (auto& callback : callbacks[event.eventType])
    //     {
    //         callback(event);
    //     }
    // }

    // eventList.clear();
    // // 更新所有设备状态
    // for (auto& [type, devices] : m_devices) {
    //     for (auto& device : devices)
    //     {
    //         device->Update();
    //     }
    // }
}

// std::vector<InputEvent> InputSystem::GetInputEvents()
// {
//     return eventList;
// }

}  // namespace ReiToEngine
