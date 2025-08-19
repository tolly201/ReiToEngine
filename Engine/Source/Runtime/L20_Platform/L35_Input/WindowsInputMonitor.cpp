#include "L0_Macro/Include.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
#include "WindowsInputMonitor.h"

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
    case 'C': return KEY_CODE_KEYBOARD::C;
    case 'D': return KEY_CODE_KEYBOARD::D;
    case 'E': return KEY_CODE_KEYBOARD::E;
    case 'F': return KEY_CODE_KEYBOARD::F;
    case 'G': return KEY_CODE_KEYBOARD::G;
    case 'H': return KEY_CODE_KEYBOARD::H;
    case 'I': return KEY_CODE_KEYBOARD::I;
    case 'J': return KEY_CODE_KEYBOARD::J;
    case 'K': return KEY_CODE_KEYBOARD::K;
    case 'L': return KEY_CODE_KEYBOARD::L;
    case 'M': return KEY_CODE_KEYBOARD::M;
    case 'N': return KEY_CODE_KEYBOARD::N;
    case 'O': return KEY_CODE_KEYBOARD::O;
    case 'P': return KEY_CODE_KEYBOARD::P;
    case 'Q': return KEY_CODE_KEYBOARD::Q;
    case 'R': return KEY_CODE_KEYBOARD::R;
    case 'S': return KEY_CODE_KEYBOARD::S;
    case 'T': return KEY_CODE_KEYBOARD::T;
    case 'U': return KEY_CODE_KEYBOARD::U;
    case 'V': return KEY_CODE_KEYBOARD::V;
    case 'W': return KEY_CODE_KEYBOARD::W;
    case 'X': return KEY_CODE_KEYBOARD::X;
    case 'Y': return KEY_CODE_KEYBOARD::Y;
    case 'Z': return KEY_CODE_KEYBOARD::Z;
    case '0': return KEY_CODE_KEYBOARD::NUM_0;
    case '1': return KEY_CODE_KEYBOARD::NUM_1;
    case '2': return KEY_CODE_KEYBOARD::NUM_2;
    case '3': return KEY_CODE_KEYBOARD::NUM_3;
    case '4': return KEY_CODE_KEYBOARD::NUM_4;
    case '5': return KEY_CODE_KEYBOARD::NUM_5;
    case '6': return KEY_CODE_KEYBOARD::NUM_6;
    case '7': return KEY_CODE_KEYBOARD::NUM_7;
    case '8': return KEY_CODE_KEYBOARD::NUM_8;
    case '9': return KEY_CODE_KEYBOARD::NUM_9;
    case VK_SPACE: return KEY_CODE_KEYBOARD::SPACE;
    case VK_RETURN: return KEY_CODE_KEYBOARD::ENTER;
    case VK_ESCAPE: return KEY_CODE_KEYBOARD::ESCAPE;
    case VK_TAB: return KEY_CODE_KEYBOARD::TAB;
    case VK_BACK: return KEY_CODE_KEYBOARD::BACKSPACE;
    case VK_SHIFT: return KEY_CODE_KEYBOARD::LEFT_SHIFT;
    case VK_CONTROL: return KEY_CODE_KEYBOARD::LEFT_CONTROL;
    case VK_MENU: return KEY_CODE_KEYBOARD::LEFT_ALT;
    case VK_LWIN: return KEY_CODE_KEYBOARD::LEFT_SUPER;
    case VK_RWIN: return KEY_CODE_KEYBOARD::RIGHT_SUPER;
    case VK_LEFT: return KEY_CODE_KEYBOARD::LEFT;
    case VK_RIGHT: return KEY_CODE_KEYBOARD::RIGHT;
    case VK_UP: return KEY_CODE_KEYBOARD::UP;
    case VK_DOWN: return KEY_CODE_KEYBOARD::DOWN;
    case VK_DELETE: return KEY_CODE_KEYBOARD::DELETE;
    case VK_HOME: return KEY_CODE_KEYBOARD::HOME;
    case VK_END: return KEY_CODE_KEYBOARD::END;
    case VK_PRIOR: return KEY_CODE_KEYBOARD::PAGE_UP;
    case VK_NEXT: return KEY_CODE_KEYBOARD::PAGE_DOWN;
    // 可继续补充其他键
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
