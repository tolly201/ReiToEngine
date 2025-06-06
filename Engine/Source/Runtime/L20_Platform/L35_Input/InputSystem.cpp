#include "InputSystem.h"

#include "L20_Platform/L23_Logger/Include.h"
namespace ReiToEngine {

InputSystem::InputSystem()  = default;
InputSystem::~InputSystem() = default;

b8 InputSystem::Initialize() {
    RT_Platform_SYSZeroMemory(&cur_state, sizeof(cur_state));
    RT_Platform_SYSZeroMemory(&prev_state, sizeof(prev_state));

    input_monitor = new PlatformInputMonitor();
    input_monitor->Initialize();
    initialized = true;

    RT_LOG_INFO("InputSystem Initialized.");
}
b8 InputSystem::Terminate() {
    input_monitor->Terminate();
    initialized = false;
}
b8 InputSystem::Tick(f64 tick) {
    if (!initialized) {
        RT_LOG_FATAL("Update when not initialized.");
        return false;
    }
    input_monitor->Tick(tick);
    RT_Platform_SYSCopyMemory(&prev_state, &cur_state, sizeof(cur_state));
    return true;
}

b8 InputSystem::IsKeyDown(KEY_CODE_KEYBOARD keyCode) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return cur_state.keys[static_cast<u8>(keyCode)];
}
b8 InputSystem::IsKeyUp(KEY_CODE_KEYBOARD keyCode) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return !cur_state.keys[static_cast<u8>(keyCode)];
}
b8 InputSystem::WasKeyDown(KEY_CODE_KEYBOARD keyCode) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return prev_state.keys[static_cast<u8>(keyCode)];
}
b8 InputSystem::WasKeyUp(KEY_CODE_KEYBOARD keyCode) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return !prev_state.keys[static_cast<u8>(keyCode)];
}

void InputSystem::ProcessKey(KEY_CODE_KEYBOARD keycode, b8 pressed) {
    if (cur_state.keys[static_cast<u8>(keycode)] != pressed) {
        cur_state.keys[static_cast<u8>(keycode)] = pressed;

        event_context content;
        EventSystem::Instance().TriggerEvent(
            pressed ? static_cast<u32>(SYSTEM_EVENT_CODE::KEY_PRESS) : static_cast<u32>(SYSTEM_EVENT_CODE::KEY_RELEASE), this, content);
    }
}

b8 InputSystem::IsMouseButtonDown(KEY_CODE_MOUSE mouseButton) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return cur_state.mouse_buttons[static_cast<u8>(mouseButton)];
}
b8 InputSystem::IsMouseButtonUp(KEY_CODE_MOUSE mouseButton) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return !cur_state.mouse_buttons[static_cast<u8>(mouseButton)];
}
b8 InputSystem::WasMouseButtonDown(KEY_CODE_MOUSE mouseButton) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return prev_state.mouse_buttons[static_cast<u8>(mouseButton)];
}
b8 InputSystem::WasMouseButtonUp(KEY_CODE_MOUSE mouseButton) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return !prev_state.mouse_buttons[static_cast<u8>(mouseButton)];
}
void InputSystem::GetMousePosition(f32& x, f32& y) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        x = 0.0f;
        y = 0.0f;
        return;
    }
    x = cur_state.mouseX;
    y = cur_state.mouseY;
}
void InputSystem::GetPrevMousePosition(f32& x, f32& y) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        x = 0.0f;
        y = 0.0f;
        return;
    }
    x = prev_state.mouseX;
    y = prev_state.mouseY;
}

void InputSystem::ProcessMouseButton(KEY_CODE_MOUSE mouseButton, b8 pressed) {
    if (cur_state.mouse_buttons[static_cast<u8>(mouseButton)] != pressed) {
        cur_state.mouse_buttons[static_cast<u8>(mouseButton)] = pressed;

        event_context content;
        EventSystem::Instance().TriggerEvent(
            pressed ? static_cast<u32>(SYSTEM_EVENT_CODE::MOUSE_BUTTON_PRESS) : static_cast<u32>(SYSTEM_EVENT_CODE::MOUSE_BUTTON_RELEASE), this, content);
    }
}
void InputSystem::ProcessMouseMove(f32 x, f32 y) {
    if (cur_state.mouseX != x || cur_state.mouseY != y) {
        cur_state.mouseX = x;
        cur_state.mouseY = y;
        event_context content;
        content.data.f32[0] = x;
        content.data.f32[1] = y;
        EventSystem::Instance().TriggerEvent(static_cast<u32>(SYSTEM_EVENT_CODE::MOUSE_MOVE), this, content);
    }
}
void InputSystem::ProcessMouseScroll(f32 x, f32 y) {
    if (cur_state.mouse_scroll_x != x || cur_state.mouse_scroll_y != y) {
        cur_state.mouse_scroll_x = x;
        cur_state.mouse_scroll_x = y;
        event_context content;
        content.data.f32[0] = x;
        content.data.f32[1] = y;
        EventSystem::Instance().TriggerEvent(static_cast<u32>(SYSTEM_EVENT_CODE::MOUSE_WHEEL), this, content);
    }
}
} // namespace ReiToEngine
