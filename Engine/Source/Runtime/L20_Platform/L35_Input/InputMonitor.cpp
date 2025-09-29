#include "InputMonitor.h"

#include "L20_Platform/L23_Logger/Include.h"
namespace ReiToEngine {

InputMonitor::InputMonitor()  = default;
InputMonitor::~InputMonitor() = default;

b8 InputMonitor::Initialize() {
    GetMemoryManager().ZeroMemory(&cur_state, sizeof(cur_state));
    GetMemoryManager().ZeroMemory(&prev_state, sizeof(prev_state));
    initialized = true;

    RT_LOG_INFO("InputMonitor Initialized.");
    return true;
}
b8 InputMonitor::Terminate() {
    initialized = false;
    return true;
}
b8 InputMonitor::Tick(f64) {
    if (!initialized) {
        RT_LOG_FATAL("Update when not initialized.");
        return false;
    }
    GetMemoryManager().CopyMemory(&prev_state, &cur_state, sizeof(cur_state));
    GetMemoryManager().ZeroMemory(&cur_state, sizeof(cur_state));
    return true;
}

b8 InputMonitor::IsKeyDown(KEY_CODE_KEYBOARD keyCode) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return cur_state.keys[static_cast<u8>(keyCode)];
}
b8 InputMonitor::IsKeyUp(KEY_CODE_KEYBOARD keyCode) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return !cur_state.keys[static_cast<u8>(keyCode)];
}
b8 InputMonitor::WasKeyDown(KEY_CODE_KEYBOARD keyCode) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return prev_state.keys[static_cast<u8>(keyCode)];
}
b8 InputMonitor::WasKeyUp(KEY_CODE_KEYBOARD keyCode) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return !prev_state.keys[static_cast<u8>(keyCode)];
}

void InputMonitor::ProcessKey(InputEvent input_event, b8 pressed) {
    if (cur_state.keys[static_cast<u8>(input_event.eventType)] != pressed) {
        cur_state.keys[static_cast<u8>(input_event.eventType)] = pressed;

        event_context content;
        content.data.u16[0] = static_cast<u16>(input_event.eventType);
        content.data.u16[1] = static_cast<u16>(input_event.modifiers);
        content.data.u16[2] = static_cast<u16>(input_event.data.keyboard.keyCode);
        content.user_data = &input_event;
        EventSystem::Instance().TriggerEvent(
            pressed ? static_cast<u32>(SYSTEM_EVENT_CODE::KEY_PRESS) : static_cast<u32>(SYSTEM_EVENT_CODE::KEY_RELEASE), this, content);
    }
}

b8 InputMonitor::IsMouseButtonDown(KEY_CODE_MOUSE mouseButton) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return cur_state.mouse_buttons[static_cast<u8>(mouseButton)];
}
b8 InputMonitor::IsMouseButtonUp(KEY_CODE_MOUSE mouseButton) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return !cur_state.mouse_buttons[static_cast<u8>(mouseButton)];
}
b8 InputMonitor::WasMouseButtonDown(KEY_CODE_MOUSE mouseButton) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return prev_state.mouse_buttons[static_cast<u8>(mouseButton)];
}
b8 InputMonitor::WasMouseButtonUp(KEY_CODE_MOUSE mouseButton) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        return false;
    }
    return !prev_state.mouse_buttons[static_cast<u8>(mouseButton)];
}
void InputMonitor::GetMousePosition(f32& x, f32& y) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        x = 0.0f;
        y = 0.0f;
        return;
    }
    x = cur_state.mouseX;
    y = cur_state.mouseY;
}
void InputMonitor::GetPrevMousePosition(f32& x, f32& y) {
    if (!initialized) {
        RT_LOG_ERROR("IsKeyDown when not initialized.");
        x = 0.0f;
        y = 0.0f;
        return;
    }
    x = prev_state.mouseX;
    y = prev_state.mouseY;
}

void InputMonitor::ProcessMouseButton(KEY_CODE_MOUSE mouseButton, b8 pressed) {
    if (cur_state.mouse_buttons[static_cast<u8>(mouseButton)] != pressed) {
        cur_state.mouse_buttons[static_cast<u8>(mouseButton)] = pressed;

        event_context content{};
        EventSystem::Instance().TriggerEvent(
            pressed ? static_cast<u32>(SYSTEM_EVENT_CODE::MOUSE_BUTTON_PRESS) : static_cast<u32>(SYSTEM_EVENT_CODE::MOUSE_BUTTON_RELEASE), this, content);
    }
}
void InputMonitor::ProcessMouseMove(f32 x, f32 y) {
    if (cur_state.mouseX != x || cur_state.mouseY != y) {

        cur_state.mouseX = x;
        cur_state.mouseY = y;
        event_context content;
        content.data.f32[0] = x;
        content.data.f32[1] = y;
        EventSystem::Instance().TriggerEvent(static_cast<u32>(SYSTEM_EVENT_CODE::MOUSE_MOVE), this, content);
    }
}
void InputMonitor::ProcessMouseScroll(f32 x, f32 y) {
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
