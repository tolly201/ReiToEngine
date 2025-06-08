#ifndef CORE_HAL_INPUT_SYSTEM_H
#define CORE_HAL_INPUT_SYSTEM_H
#include "L0_Macro/Include.h"
#include "InputEnums.h"
#include "L20_Platform/L31_SingletonFactory/RuntimeSingleton.h"
#include "InputMonitor.h"
#ifdef RT_SYSTEM_WINDOWS
    #include "WindowsInputMonitor.h"
    using PlatformInputMonitor = ReiToEngine::WindowsInputMonitor;
#elif defined(RT_SYSTEM_APPLE)
    #include "MacOSXInputMonitor.h"
    using PlatformInputMonitor = ReiToEngine::MacOSXInputMonitor;
#elif RT_SYSTEM_LINUX
    #include "LinuxInputMonitor.h"
    using PlatformInputMonitor = LinuxInputMonitor;
#endif

namespace ReiToEngine {

class RTENGINE_API InputSystem : public Runtime_Singleton<InputSystem> {
friend class Runtime_Singleton<InputSystem>;
public:
    InputSystem();
    ~InputSystem();

    b8 Initialize();
    b8 Terminate();
    b8 Tick(f64);

    b8 IsKeyDown(KEY_CODE_KEYBOARD keyCode);
    b8 IsKeyUp(KEY_CODE_KEYBOARD keyCode);
    b8 WasKeyDown(KEY_CODE_KEYBOARD keyCode);
    b8 WasKeyUp(KEY_CODE_KEYBOARD keyCode);

    void ProcessKey(KEY_CODE_KEYBOARD keycode, b8 pressed);

    b8 IsMouseButtonDown(KEY_CODE_MOUSE mouseButton);
    b8 IsMouseButtonUp(KEY_CODE_MOUSE mouseButton);
    b8 WasMouseButtonDown(KEY_CODE_MOUSE mouseButton);
    b8 WasMouseButtonUp(KEY_CODE_MOUSE mouseButton);
    void GetMousePosition(f32& x, f32& y);
    void GetPrevMousePosition(f32& x, f32& y);

    void ProcessMouseButton(KEY_CODE_MOUSE mouseButton, b8 pressed);
    void ProcessMouseMove(f32 x, f32 y);
    void ProcessMouseScroll(f32 x, f32 y);

    std::vector<InputEvent> GetInputEvents();
   private:
    struct InputState
    {
        b8 keys[static_cast<u8>(KEY_CODE_KEYBOARD::MAX)];
        b8 mouse_buttons[static_cast<u8>(KEY_CODE_MOUSE::MAX)];

        f32 mouseX;
        f32 mouseY;

        f32 mouse_scroll_x;
        f32 mouse_scroll_y;
    };

    InputState cur_state;
    InputState prev_state;
    b8 initialized = false;

    PlatformInputMonitor* input_monitor;
};

}  // namespace ReiToEngine
#endif
