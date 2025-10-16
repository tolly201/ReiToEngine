#include "InputSystem.h"

#include "L20_Platform/L21_Logger/Include.h"
namespace ReiToEngine {

InputSystem::InputSystem()  = default;
InputSystem::~InputSystem() = default;

b8 InputSystem::Initialize() {
    // global_input_monitor = new PlatformInputMonitor();
    // global_input_monitor->Initialize();
    initialized = true;

    RT_LOG_INFO_PLATFORM("InputSystem Initialized.");
    return true;
}
b8 InputSystem::Terminate() {
    // global_input_monitor->Terminate();
    for(auto& monitor : input_monitors) {
        monitor.second->Terminate();
    }

    initialized = false;
    return true;
}
b8 InputSystem::Tick(f64 tick) {
    if (!initialized) {
        RT_LOG_FATAL_PLATFORM("Update when not initialized.");
        return false;
    }
    for (auto& monitor : input_monitors) {
        if (!monitor.second->Tick(tick)) {
            RT_LOG_ERROR_PLATFORM("Failed to tick input monitor for handle: ", monitor.first);
        }
    }
    // global_input_monitor->Tick(tick);
    return true;
}

PlatformInputMonitor* InputSystem::CreateOrGetMonitor(void* window)
{
    input_monitors[window] = new PlatformInputMonitor();
    if (!input_monitors[window]->Initialize()) {
        RT_LOG_ERROR_PLATFORM("Failed to initialize input monitor for handle: ", window);
        return nullptr;
    }
    return input_monitors[window];
}
b8 InputSystem::DestroyMonitor(void* window)
{
    PlatformInputMonitor* monitor = input_monitors[window];
    if (monitor) {
        monitor->Terminate();
        delete monitor;
        input_monitors.erase(window);
        RT_LOG_INFO_PLATFORM("Input monitor for handle: ", window, " destroyed.");
        return true;
    }
    return false;
}

} // namespace ReiToEngine
