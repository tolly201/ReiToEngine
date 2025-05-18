#ifndef PLATFORM_HAL_INPUT_ENUMS_H
#define PLATFORM_HAL_INPUT_ENUMS_H

#include <array>
#include <cstdint>
#include <functional>
namespace ReiToEngine {

enum class EINPUT_DEVICE_TYPE : uint8_t {
    DEVICE_UNKNOWN = 0,
    DEVICE_KEYBOARD = 1,
    DEVICE_MOUSE = 2,
    DEVICE_GAMEPAD = 3,
    DEVICE_TOUCH = 4,
    //add
};

enum class EINPUT_EVENT_TYPE : uint8_t {
    EVENT_UNKNOWN = 0,
    EVENT_KEY_PRESS = 1,
    EVENT_KEY_RELEASE = 2,
    EVENT_KEY_REPEAT = 3,
    EVENT_TEXT_INPUT = 4,
    EVENT_POINTER_DOWN = 5,
    EVENT_POINTER_UP = 6,
    EVENT_POINTER_MOVE = 7,
    EVENT_SCROLL = 8,
    EVENT_DEVICE_CONNECTED = 9,
    EVENT_DEVICE_DISCONNECTED = 10
};

enum class EINPUT_KEY_CODE : uint16_t {
    INPUT_UNKNOWN                = 0,
    INPUT_KEYBOARD_SPACE         = 32,
    INPUT_KEYBOARD_APOSTROPHE    = 39,
    INPUT_KEYBOARD_COMMA         = 44,
    INPUT_KEYBOARD_MINUS         = 45,
    INPUT_KEYBOARD_PERIOD        = 46,
    INPUT_KEYBOARD_SLASH         = 47,
    INPUT_KEYBOARD_0             = 48,
    INPUT_KEYBOARD_1             = 49,
    INPUT_KEYBOARD_2             = 50,
    INPUT_KEYBOARD_3             = 51,
    INPUT_KEYBOARD_4             = 52,
    INPUT_KEYBOARD_5             = 53,
    INPUT_KEYBOARD_6             = 54,
    INPUT_KEYBOARD_7             = 55,
    INPUT_KEYBOARD_8             = 56,
    INPUT_KEYBOARD_9             = 57,
    INPUT_KEYBOARD_SEMICOLON     = 59,
    INPUT_KEYBOARD_EQUAL         = 61,
    INPUT_KEYBOARD_A             = 65,
    INPUT_KEYBOARD_B             = 66,
    INPUT_KEYBOARD_C             = 67,
    INPUT_KEYBOARD_D             = 68,
    INPUT_KEYBOARD_E             = 69,
    INPUT_KEYBOARD_F             = 70,
    INPUT_KEYBOARD_G             = 71,
    INPUT_KEYBOARD_H             = 72,
    INPUT_KEYBOARD_I             = 73,
    INPUT_KEYBOARD_J             = 74,
    INPUT_KEYBOARD_K             = 75,
    INPUT_KEYBOARD_L             = 76,
    INPUT_KEYBOARD_M             = 77,
    INPUT_KEYBOARD_N             = 78,
    INPUT_KEYBOARD_O             = 79,
    INPUT_KEYBOARD_P             = 80,
    INPUT_KEYBOARD_Q             = 81,
    INPUT_KEYBOARD_R             = 82,
    INPUT_KEYBOARD_S             = 83,
    INPUT_KEYBOARD_T             = 84,
    INPUT_KEYBOARD_U             = 85,
    INPUT_KEYBOARD_V             = 86,
    INPUT_KEYBOARD_W             = 87,
    INPUT_KEYBOARD_X             = 88,
    INPUT_KEYBOARD_Y             = 89,
    INPUT_KEYBOARD_Z             = 90,
    INPUT_KEYBOARD_LEFT_BRACKET  = 91,
    INPUT_KEYBOARD_BACKSLASH     = 92,
    INPUT_KEYBOARD_RIGHT_BRACKET = 93,
    INPUT_KEYBOARD_GRAVE_ACCENT  = 96,
    INPUT_KEYBOARD_WORLD_1       = 161,
    INPUT_KEYBOARD_WORLD_2       = 162,

    INPUT_KEYBOARD_ESCAPE        = 256,
    INPUT_KEYBOARD_ENTER         = 257,
    INPUT_KEYBOARD_TAB           = 258,
    INPUT_KEYBOARD_BACKSPACE     = 259,
    INPUT_KEYBOARD_INSERT        = 260,
    INPUT_KEYBOARD_DELETE        = 261,
    INPUT_KEYBOARD_RIGHT         = 262,
    INPUT_KEYBOARD_LEFT          = 263,
    INPUT_KEYBOARD_DOWN          = 264,
    INPUT_KEYBOARD_UP            = 265,
    INPUT_KEYBOARD_PAGE_UP       = 266,
    INPUT_KEYBOARD_PAGE_DOWN     = 267,
    INPUT_KEYBOARD_HOME          = 268,
    INPUT_KEYBOARD_END           = 269,
    INPUT_KEYBOARD_CAPS_LOCK     = 280,
    INPUT_KEYBOARD_SCROLL_LOCK   = 281,
    INPUT_KEYBOARD_NUM_LOCK      = 282,
    INPUT_KEYBOARD_PRINT_SCREEN  = 283,
    INPUT_KEYBOARD_PAUSE         = 284,
    INPUT_KEYBOARD_F1            = 290,
    INPUT_KEYBOARD_F2            = 291,
    INPUT_KEYBOARD_F3            = 292,
    INPUT_KEYBOARD_F4            = 293,
    INPUT_KEYBOARD_F5            = 294,
    INPUT_KEYBOARD_F6            = 295,
    INPUT_KEYBOARD_F7            = 296,
    INPUT_KEYBOARD_F8            = 297,
    INPUT_KEYBOARD_F9            = 298,
    INPUT_KEYBOARD_F10           = 299,
    INPUT_KEYBOARD_F11           = 300,
    INPUT_KEYBOARD_F12           = 301,
    INPUT_KEYBOARD_F13           = 302,
    INPUT_KEYBOARD_F14           = 303,
    INPUT_KEYBOARD_F15           = 304,
    INPUT_KEYBOARD_F16           = 305,
    INPUT_KEYBOARD_F17           = 306,
    INPUT_KEYBOARD_F18           = 307,
    INPUT_KEYBOARD_F19           = 308,
    INPUT_KEYBOARD_F20           = 309,
    INPUT_KEYBOARD_F21           = 310,
    INPUT_KEYBOARD_F22           = 311,
    INPUT_KEYBOARD_F23           = 312,
    INPUT_KEYBOARD_F24           = 313,
    INPUT_KEYBOARD_F25           = 314,
    INPUT_KEYBOARD_KP_0          = 320,
    INPUT_KEYBOARD_KP_1          = 321,
    INPUT_KEYBOARD_KP_2          = 322,
    INPUT_KEYBOARD_KP_3          = 323,
    INPUT_KEYBOARD_KP_4          = 324,
    INPUT_KEYBOARD_KP_5          = 325,
    INPUT_KEYBOARD_KP_6          = 326,
    INPUT_KEYBOARD_KP_7          = 327,
    INPUT_KEYBOARD_KP_8          = 328,
    INPUT_KEYBOARD_KP_9          = 329,
    INPUT_KEYBOARD_KP_DECIMAL    = 330,
    INPUT_KEYBOARD_KP_DIVIDE     = 331,
    INPUT_KEYBOARD_KP_MULTIPLY   = 332,
    INPUT_KEYBOARD_KP_SUBTRACT   = 333,
    INPUT_KEYBOARD_KP_ADD        = 334,
    INPUT_KEYBOARD_KP_ENTER      = 335,
    INPUT_KEYBOARD_KP_EQUAL      = 336,
    INPUT_KEYBOARD_LEFT_SHIFT    = 340,
    INPUT_KEYBOARD_LEFT_CONTROL  = 341,
    INPUT_KEYBOARD_LEFT_ALT      = 342,
    INPUT_KEYBOARD_LEFT_SUPER    = 343,
    INPUT_KEYBOARD_RIGHT_SHIFT   = 344,
    INPUT_KEYBOARD_RIGHT_CONTROL = 345,
    INPUT_KEYBOARD_RIGHT_ALT     = 346,
    INPUT_KEYBOARD_RIGHT_SUPER   = 347,
    INPUT_KEYBOARD_MENU          = 348,
    // reset until
    // 0x0FFF-> 4095

    // Mouse buttons
    INPUT_MOUSE_BUTTON_LEFT = 0x1000,
    INPUT_MOUSE_BUTTON_RIGHT,
    INPUT_MOUSE_BUTTON_MIDDLE,
    INPUT_MOUSE_BUTTON_4,
    INPUT_MOUSE_BUTTON_5,
    INPUT_MOUSE_BUTTON_6,

    // Gamepad buttons could go here
    INPUT_GAMEPAD_BUTTON_A = 0x2000,
    //todo full input key and devices

};

// Modifier flags (same as before)
enum class EINPUT_MODIFIER : uint16_t {
    MOD_NONE = 0x0000,
    MOD_KEYBOARD_SHIFT = 0x0001,
    MOD_KEYBOARD_CONTROL = 0x0002,
    MOD_KEYBOARD_ALT = 0x0004,
    MOD_KEYBOARD_SUPER = 0x0008,
    MOD_KEYBOARD_CAPS_LOCK = 0x0010,
    MOD_KEYBOARD_NUM_LOCK = 0x0020
};


struct InputEvent
{
    EINPUT_DEVICE_TYPE deviceType;
    EINPUT_EVENT_TYPE eventType;
    EINPUT_KEY_CODE inputCode;
    EINPUT_MODIFIER modifiers;

    struct position {
        float x;
        float y;
    } ;
    // Text input
    uint32_t codepoint;

    // Timestamp
    uint64_t timestamp;
    float scrollDeltaX;
    float scrollDeltaY;
    float positionX;
    float positionY;
};

constexpr std::array<EINPUT_DEVICE_TYPE, 4> ALL_DEVICE_TYPES=
{
    EINPUT_DEVICE_TYPE::DEVICE_KEYBOARD,
    EINPUT_DEVICE_TYPE::DEVICE_MOUSE,
    EINPUT_DEVICE_TYPE::DEVICE_GAMEPAD,
    EINPUT_DEVICE_TYPE::DEVICE_TOUCH,
};

using InputCallback = std::function<void(const InputEvent& event)>;
} // namespace ReiToEngine
#endif
