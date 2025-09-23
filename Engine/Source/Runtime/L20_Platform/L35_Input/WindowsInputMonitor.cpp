#include "L0_Macro/Include.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
#include "WindowsInputMonitor.h"
#include <windowsx.h>
namespace ReiToEngine {

WindowsInputMonitor::WindowsInputMonitor()
{
}

WindowsInputMonitor::~WindowsInputMonitor()
{
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
        ProcessKey(inputEvent, true);
        break;

    // 键盘释放
    case WM_KEYUP:
    case WM_SYSKEYUP:
        inputEvent.deviceType = INPUT_DEVICE_TYPE::KEYBOARD;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::KEY_RELEASE;
        inputEvent.data.keyboard.keyCode = MapKeyCode(msg->wParam);
        inputEvent.modifiers = GetModifiers();
        ProcessKey(inputEvent, false);
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
}
} // namespace ReiToEngine
#endif
