#include "../Include/InputSystem.h"

#include "Platform/HAL/Input/Include/InputEnums.h"

#ifdef RT_SYSTEM_APPLE
    #include "Platform/HAL/Input/Include/MacOSXInputMonitor.h"
#endif

namespace ReiToEngine {

void ReceiveEventFromMonitor(const InputEvent& event)
{
    InputSystem::Instance().ProcessInputEvent(event);
}

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::AddInputCallback(EINPUT_EVENT_TYPE type, InputCallback callback)
{
    printf("add call back");
    if (callbacks.find(type) == callbacks.end())
    {
        callbacks[type] = std::vector<InputCallback>();
    }

    callbacks[type].push_back(callback);
    printf("add call back%hhu,%lu",type,callbacks[type].size());
}
void InputSystem::RemoveInputCallbacks(EINPUT_EVENT_TYPE type)
{
    if (callbacks.find(type) != callbacks.end())
    {
        callbacks[type].clear();
    }
}

void InputSystem::ProcessInputEvent(const InputEvent& event)
{
    eventList.push_back(event);
}

void InputSystem::Initialize()
{
    #ifdef RT_SYSTEM_APPLE
    m_eventMonitor = new MacOSXInputMonitor();
    #endif

    m_eventMonitor->AddInputCallback(ReceiveEventFromMonitor);
    m_eventMonitor->Activate();
}

void InputSystem::Terminate()
{

}

void InputSystem::Tick()
{
    for (auto& event : eventList)
    {
        printf("input system tick, eventList: %lu\n", eventList.size());
        printf("eventType: %hhu\n", event.eventType);
        printf("event Code: %u\n", event.codepoint);
        printf("event num: %hu\n", event.inputCode);
        for (auto& callback : callbacks[event.eventType])
        {
            callback(event);
        }
    }

    eventList.clear();
    // 更新所有设备状态
    for (auto& [type, devices] : m_devices) {
        for (auto& device : devices)
        {
            device->Update();
        }
    }
}

}  // namespace ReiToEngine
