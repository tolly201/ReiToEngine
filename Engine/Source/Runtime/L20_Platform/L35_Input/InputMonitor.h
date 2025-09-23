#ifndef PLATFORM_INPUT_MONITOR_H
#define PLATFORM_INPUT_MONITOR_H
#include "L0_Macro/Include.h"
#include "InputEnums.h"
namespace ReiToEngine {

class RTENGINE_API InputMonitor{
public:
    InputMonitor();
    ~InputMonitor();

    b8 Initialize();
    b8 Terminate();
    b8 Tick(f64);

    b8 IsKeyDown(KEY_CODE_KEYBOARD keyCode);
    b8 IsKeyUp(KEY_CODE_KEYBOARD keyCode);
    b8 WasKeyDown(KEY_CODE_KEYBOARD keyCode);
    b8 WasKeyUp(KEY_CODE_KEYBOARD keyCode);

    void ProcessKey(InputEvent input_event, b8 pressed);

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
    b8 isActive;
};

}  // namespace ReiToEngine
#endif
