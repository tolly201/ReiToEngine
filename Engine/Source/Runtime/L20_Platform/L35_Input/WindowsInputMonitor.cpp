#include "L0_Macro/Include.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
#include "WindowsInputMonitor.h"

namespace ReiToEngine {

WindowsInputMonitor::WindowsInputMonitor(HWND hwnd)
    : hwnd_(hwnd)
{
    // 可以在这里注册原生窗口过程（或保存回调）
    // 例如：SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
    // 或者在窗口消息循环中调用 ProcessNativeEvent
}

WindowsInputMonitor::~WindowsInputMonitor()
{
    // 清理资源（如取消窗口过程等）
}

void WindowsInputMonitor::ProcessNativeEvent(MSG* msg)
{
    InputEvent inputEvent;
    inputEvent.deviceType = INPUT_DEVICE_TYPE::UNKNOWN;
    inputEvent.eventType  = SYSTEM_EVENT_CODE::UNKNOWN;
    inputEvent.modifiers  = KEY_CODE_MODIFIER::NONE;

    switch (msg->message)
    {
    // 键盘按下
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        inputEvent.deviceType = INPUT_DEVICE_TYPE::KEYBOARD;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::KEY_PRESS;
        inputEvent.data.keyboard.keyCode = MapKeyCode(msg->wParam);
        // 可补充修饰键状态
        inputEvent.modifiers = GetModifiers();
        ProcessKey(inputEvent.data.keyboard.keyCode, true);
        break;

    // 键盘释放
    case WM_KEYUP:
    case WM_SYSKEYUP:
        inputEvent.deviceType = INPUT_DEVICE_TYPE::KEYBOARD;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::KEY_RELEASE;
        inputEvent.data.keyboard.keyCode = MapKeyCode(msg->wParam);
        inputEvent.modifiers = GetModifiers();
        ProcessKey(inputEvent.data.keyboard.keyCode, false);
        break;

    // 鼠标移动
    case WM_MOUSEMOVE:
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_MOVE;
        inputEvent.data.mouse.positionX = GET_X_LPARAM(msg->lParam);
        inputEvent.data.mouse.positionY = GET_Y_LPARAM(msg->lParam);
        ProcessMouseMove(inputEvent.data.mouse.positionX, inputEvent.data.mouse.positionY);
        break;

    // 鼠标滚轮
    case WM_MOUSEWHEEL:
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_WHEEL;
        inputEvent.data.mouse.scrollDeltaY = GET_WHEEL_DELTA_WPARAM(msg->wParam);
        ProcessMouseScroll(0, inputEvent.data.mouse.scrollDeltaY);
        break;

    // 鼠标按键
    case WM_LBUTTONDOWN:
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_BUTTON_PRESS;
        ProcessMouseButton(KEY_CODE_MOUSE::BUTTON_LEFT, true);
        break;
    case WM_LBUTTONUP:
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_BUTTON_RELEASE;
        ProcessMouseButton(KEY_CODE_MOUSE::BUTTON_LEFT, false);
        break;
    case WM_RBUTTONDOWN:
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_BUTTON_PRESS;
        ProcessMouseButton(KEY_CODE_MOUSE::BUTTON_RIGHT, true);
        break;
    case WM_RBUTTONUP:
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_BUTTON_RELEASE;
        ProcessMouseButton(KEY_CODE_MOUSE::BUTTON_RIGHT, false);
        break;
    // 可补充其他鼠标按键和事件

    default:
        break;
    }

    // 可在此处调用输入系统的回调
    // if (isActive && inputCallback != nullptr) {
    //     inputCallback(inputEvent);
    // }
}

KEY_CODE_KEYBOARD WindowsInputMonitor::MapKeyCode(WPARAM wParam)
{
    // 你可以根据实际需要做映射
    // 例如：VK_A -> KEY_CODE_KEYBOARD::A
    // 这里只做简单示例
    switch (wParam)
    {
    case 'A': return KEY_CODE_KEYBOARD::A;
    case 'B': return KEY_CODE_KEYBOARD::B;
    // ...补充其他键
    default: return KEY_CODE_KEYBOARD::UNKNOWN;
    }
}

KEY_CODE_MODIFIER WindowsInputMonitor::GetModifiers()
{
    KEY_CODE_MODIFIER mods = KEY_CODE_MODIFIER::NONE;
    if (GetKeyState(VK_SHIFT) & 0x8000) mods |= KEY_CODE_MODIFIER::SHIFT;
    if (GetKeyState(VK_CONTROL) & 0x8000) mods |= KEY_CODE_MODIFIER::CONTROL;
    if (GetKeyState(VK_MENU) & 0x8000) mods |= KEY_CODE_MODIFIER::ALT;
    if (GetKeyState(VK_LWIN) & 0x8000 || GetKeyState(VK_RWIN) & 0x8000) mods |= KEY_CODE_MODIFIER::SUPER;
    return mods;
}

} // namespace ReiToEngine
#endif
