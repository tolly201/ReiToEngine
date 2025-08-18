#include "InputSystem.h"

#include "L20_Platform/L23_Logger/Include.h"
namespace ReiToEngine {

InputSystem::InputSystem()  = default;
InputSystem::~InputSystem() = default;

b8 InputSystem::Initialize() {
    global_input_monitor = new PlatformInputMonitor();
    global_input_monitor->Initialize();
    initialized = true;

    RT_LOG_INFO("InputSystem Initialized.");
    return true;
}
b8 InputSystem::Terminate() {
    global_input_monitor->Terminate();
    for(auto& monitor : input_monitors) {
        monitor.second->Terminate();
    }

    initialized = false;
    return true;
}
b8 InputSystem::Tick(f64 tick) {
    if (!initialized) {
        RT_LOG_FATAL("Update when not initialized.");
        return false;
    }
    global_input_monitor->Tick(tick);
    return true;
}

b8 InputSystem::CreateOrGetMonitor(NativeHandle handle)
{
    input_monitors[handle] = new PlatformInputMonitor();
    if (!input_monitors[handle]->Initialize()) {
        RT_LOG_ERROR("Failed to initialize input monitor for handle: ", handle);
        return false;
    }
    return true;
}
b8 InputSystem::DestroyMonitor(NativeHandle handle)
{
    PlatformInputMonitor* monitor = input_monitors[handle];
    if (monitor) {
        monitor->Terminate();
        delete monitor;
        input_monitors.erase(handle);
        RT_LOG_INFO("Input monitor for handle: ", handle, " destroyed.");
        return true;
    }
}

} // namespace ReiToEngine
