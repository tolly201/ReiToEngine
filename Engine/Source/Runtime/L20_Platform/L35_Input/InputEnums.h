#ifndef PLATFORM_HAL_INPUT_ENUMS_H
#define PLATFORM_HAL_INPUT_ENUMS_H

#include "L0_Macro/Include.h"
#include "L20_Platform/L25_Event/EventSystem.h"
namespace ReiToEngine {

enum class INPUT_DEVICE_TYPE : u8 {
    UNKNOWN = 0x00,
    KEYBOARD = 0x01,
    MOUSE = 0x02,
    GAMEPAD = 0x03,
    TOUCH = 0x04,
    MAX = 0xFF
    //add
};

enum class KEY_CODE_KEYBOARD : u8 {
    UNKNOWN       = 0x00,
    SPACE         = 0x01,
    APOSTROPHE    = 0x02,
    COMMA         = 0x03,
    MINUS         = 0x04,
    PERIOD        = 0x05,
    SLASH         = 0x06,
    NUM_0         = 0x07,
    NUM_1         = 0x08,
    NUM_2         = 0x09,
    NUM_3         = 0x0A,
    NUM_4         = 0x0B,
    NUM_5         = 0x0C,
    NUM_6         = 0x0D,
    NUM_7         = 0x0E,
    NUM_8         = 0x0F,
    NUM_9         = 0x10,
    SEMICOLON     = 0x11,
    EQUAL         = 0x12,
    A             = 0x13,
    B             = 0x14,
    C             = 0x15,
    D             = 0x16,
    E             = 0x17,
    F             = 0x18,
    G             = 0x19,
    H             = 0x1A,
    I             = 0x1B,
    J             = 0x1C,
    K             = 0x1D,
    L             = 0x1E,
    M             = 0x1F,
    N             = 0x20,
    O             = 0x21,
    P             = 0x22,
    Q             = 0x23,
    R             = 0x24,
    S             = 0x25,
    T             = 0x26,
    U             = 0x27,
    V             = 0x28,
    W             = 0x29,
    X             = 0x2A,
    Y             = 0x2B,
    Z             = 0x2C,
    LEFT_BRACKET  = 0x2D,
    BACKSLASH     = 0x2E,
    RIGHT_BRACKET = 0x2F,
    GRAVE_ACCENT  = 0x30,
    WORLD_1       = 0x31,
    WORLD_2       = 0x32,

    ESCAPE        = 0x33,
    ENTER         = 0x34,
    TAB           = 0x35,
    BACKSPACE     = 0x36,
    INSERT        = 0x37,
    KEY_DELETE        = 0x38,
    RIGHT         = 0x39,
    LEFT          = 0x3A,
    DOWN          = 0x3B,
    UP            = 0x3C,
    PAGE_UP       = 0x3D,
    PAGE_DOWN     = 0x3E,
    HOME          = 0x3F,
    END           = 0x40,
    CAPS_LOCK     = 0x41,
    SCROLL_LOCK   = 0x42,
    NUM_LOCK      = 0x43,
    PRINT_SCREEN  = 0x44,
    PAUSE         = 0x45,
    F1            = 0x46,
    F2            = 0x47,
    F3            = 0x48,
    F4            = 0x49,
    F5            = 0x4A,
    F6            = 0x4B,
    F7            = 0x4C,
    F8            = 0x4D,
    F9            = 0x4E,
    F10           = 0x4F,
    F11           = 0x50,
    F12           = 0x51,
    F13           = 0x52,
    F14           = 0x53,
    F15           = 0x54,
    F16           = 0x55,
    F17           = 0x56,
    F18           = 0x57,
    F19           = 0x58,
    F20           = 0x59,
    F21           = 0x5A,
    F22           = 0x5B,
    F23           = 0x5C,
    F24           = 0x5D,
    F25           = 0x5E,
    KP_0          = 0x5F,
    KP_1          = 0x60,
    KP_2          = 0x61,
    KP_3          = 0x62,
    KP_4          = 0x63,
    KP_5          = 0x64,
    KP_6          = 0x65,
    KP_7          = 0x66,
    KP_8          = 0x67,
    KP_9          = 0x68,
    KP_DECIMAL    = 0x69,
    KP_DIVIDE     = 0x6A,
    KP_MULTIPLY   = 0x6B,
    KP_SUBTRACT   = 0x6C,
    KP_ADD        = 0x6D,
    KP_ENTER      = 0x6E,
    KP_EQUAL      = 0x6F,
    LEFT_SHIFT    = 0x70,
    LEFT_CONTROL  = 0x71,
    LEFT_ALT      = 0x72,
    LEFT_SUPER    = 0x73,
    RIGHT_SHIFT   = 0x74,
    RIGHT_CONTROL = 0x75,
    RIGHT_ALT     = 0x76,
    RIGHT_SUPER   = 0x77,
    MENU          = 0x78,
    MAX           = 0xFF
};
enum class KEY_CODE_MODIFIER : u16
{
    NONE      = 0x00,
    SHIFT     = 0x01,
    CONTROL   = 0x02,
    ALT       = 0x04,
    SUPER     = 0x08,
    CAPS_LOCK = 0x10,
    NUM_LOCK  = 0x20,
    LEFT_ALT  = 0x40,
    RIGHT_ALT = 0x80,
    LEFT_SHIFT = 0x100,
    RIGHT_SHIFT = 0x200,
    LEFT_CONTROL = 0x400,
    RIGHT_CONTROL = 0x800,
    ALL       = 0xFF
};

inline KEY_CODE_MODIFIER operator|(KEY_CODE_MODIFIER a, KEY_CODE_MODIFIER b)
{
    return static_cast<KEY_CODE_MODIFIER>(
        static_cast<u8>(a) | static_cast<u8>(b)
    );
}
inline KEY_CODE_MODIFIER& operator|=(KEY_CODE_MODIFIER& a, KEY_CODE_MODIFIER b)
{
    a = a | b;
    return a;
}

enum class KEY_CODE_MOUSE : u8
{
    // Mouse buttons
    NONE  = 0x00,
    BUTTON_LEFT      = 0x01,
    BUTTON_RIGHT     = 0x02,
    BUTTON_MIDDLE    = 0x03,
    BUTTON_4         = 0x04,
    BUTTON_5         = 0x05,
    BUTTON_6         = 0x06,
    MAX              = 0xFF
};
enum class KEY_CODE_GAMEPAD : u8
{
    BUTTON_A       = 0x00,
};

struct KeyboardInputData {
    KEY_CODE_KEYBOARD keyCode;
};
struct TextInputData{
    u16 codepoint;
};
struct MouseInputData {
    KEY_CODE_MOUSE mouseButton;
    f32 scrollDeltaX;
    f32 scrollDeltaY;
    f32 positionX;
    f32 positionY;
};

struct GamepadInputData {
    KEY_CODE_GAMEPAD gamepadButton;
};

struct TouchInputData {
    // TOUCH_EVENT_TYPE touchType;
    u32 touchId;
    f32 positionX;
    f32 positionY;
};
union InputEventData {
    KeyboardInputData keyboard;
    TextInputData text;
    MouseInputData mouse;
    GamepadInputData gamepad;
    TouchInputData touch;
};

struct InputEvent {
    INPUT_DEVICE_TYPE deviceType;
    SYSTEM_EVENT_CODE eventType;
    KEY_CODE_MODIFIER modifiers;
    InputEventData data;
    u64 timestamp;
};
}
#endif
