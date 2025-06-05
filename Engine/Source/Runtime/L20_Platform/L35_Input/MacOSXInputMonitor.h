// #ifndef CORE_PLATFORM_HAL_MACOSX_INPUT_MONITOR_H
// #define CORE_PLATFORM_HAL_MACOSX_INPUT_MONITOR_H
// #include "L0_Macro/Include.h"
// #ifdef RT_SYSTEM_APPLE
// #import <Cocoa/Cocoa.h>
// #include "L60_Core/HAL/Input/Include/InputEnums.h"
// #include "InputMonitor.h"

// namespace ReiToEngine {
// class MacOSXInputMonitor;
// }

// @interface NSInputMonitor : NSObject
// {
//     ReiToEngine::MacOSXInputMonitor* _monitor;
// }
// - (void)addMonitor:(ReiToEngine::MacOSXInputMonitor*)Monitor;
// - (void)startMonitoring;
// @end

// namespace ReiToEngine {
// class MacOSXInputMonitor : public InputMonitor
// {
// public:
//     MacOSXInputMonitor();
//     ~MacOSXInputMonitor();
//     void InnerConvertNSEvent(NSEvent* event);
// protected:
//     NSInputMonitor* cocoaInputMonitor;

//     const std::unordered_map<uint16_t, EINPUT_KEY_CODE> keyCodeMap = {
//     {0x00, EINPUT_KEY_CODE::INPUT_KEYBOARD_A},      // 'A' key
//     {0x01, EINPUT_KEY_CODE::INPUT_KEYBOARD_S},      // 'S' key
//     {0x02, EINPUT_KEY_CODE::INPUT_KEYBOARD_D},      // 'D' key
//     {0x03, EINPUT_KEY_CODE::INPUT_KEYBOARD_F},      // 'F' key
//     {0x04, EINPUT_KEY_CODE::INPUT_KEYBOARD_H},      // 'H' key
//     {0x05, EINPUT_KEY_CODE::INPUT_KEYBOARD_G},      // 'G' key
//     {0x06, EINPUT_KEY_CODE::INPUT_KEYBOARD_Z},      // 'Z' key
//     {0x07, EINPUT_KEY_CODE::INPUT_KEYBOARD_X},      // 'X' key
//     {0x08, EINPUT_KEY_CODE::INPUT_KEYBOARD_C},      // 'C' key
//     {0x09, EINPUT_KEY_CODE::INPUT_KEYBOARD_V},      // 'V' key
//     {0x0B, EINPUT_KEY_CODE::INPUT_KEYBOARD_B},      // 'B' key
//     {0x0C, EINPUT_KEY_CODE::INPUT_KEYBOARD_Q},      // 'Q' key
//     {0x0D, EINPUT_KEY_CODE::INPUT_KEYBOARD_W},      // 'W' key
//     {0x0E, EINPUT_KEY_CODE::INPUT_KEYBOARD_E},      // 'E' key
//     {0x0F, EINPUT_KEY_CODE::INPUT_KEYBOARD_R},      // 'R' key
//     {0x10, EINPUT_KEY_CODE::INPUT_KEYBOARD_Y},      // 'Y' key
//     {0x11, EINPUT_KEY_CODE::INPUT_KEYBOARD_T},      // 'T' key
//     {0x12, EINPUT_KEY_CODE::INPUT_KEYBOARD_1},      // '1' key
//     {0x13, EINPUT_KEY_CODE::INPUT_KEYBOARD_2},      // '2' key
//     {0x14, EINPUT_KEY_CODE::INPUT_KEYBOARD_3},      // '3' key
//     {0x15, EINPUT_KEY_CODE::INPUT_KEYBOARD_4},      // '4' key
//     {0x16, EINPUT_KEY_CODE::INPUT_KEYBOARD_6},      // '6' key
//     {0x17, EINPUT_KEY_CODE::INPUT_KEYBOARD_5},      // '5' key
//     {0x18, EINPUT_KEY_CODE::INPUT_KEYBOARD_KP_EQUAL}, // '=' key
//     {0x19, EINPUT_KEY_CODE::INPUT_KEYBOARD_9},      // '9' key
//     {0x1A, EINPUT_KEY_CODE::INPUT_KEYBOARD_7},      // '7' key
//     {0x1B, EINPUT_KEY_CODE::INPUT_KEYBOARD_MINUS},  // '-' key
//     {0x1C, EINPUT_KEY_CODE::INPUT_KEYBOARD_8},      // '8' key
//     {0x1D, EINPUT_KEY_CODE::INPUT_KEYBOARD_0},      // '0' key
//     {0x1E, EINPUT_KEY_CODE::INPUT_KEYBOARD_RIGHT_BRACKET}, // ']' key
//     {0x1F, EINPUT_KEY_CODE::INPUT_KEYBOARD_O},      // 'O' key
//     {0x20, EINPUT_KEY_CODE::INPUT_KEYBOARD_U},      // 'U' key
//     {0x21, EINPUT_KEY_CODE::INPUT_KEYBOARD_LEFT_BRACKET},  // '[' key
//     {0x22, EINPUT_KEY_CODE::INPUT_KEYBOARD_I},      // 'I' key
//     {0x23, EINPUT_KEY_CODE::INPUT_KEYBOARD_P},      // 'P' key
//     {0x24, EINPUT_KEY_CODE::INPUT_KEYBOARD_LEFT_ALT}, // Return key
//     {0x25, EINPUT_KEY_CODE::INPUT_KEYBOARD_L},      // 'L' key
//     {0x26, EINPUT_KEY_CODE::INPUT_KEYBOARD_J},      // 'J' key
//     {0x27, EINPUT_KEY_CODE::INPUT_KEYBOARD_APOSTROPHE},    // '\'' key
//     {0x28, EINPUT_KEY_CODE::INPUT_KEYBOARD_K},      // 'K' key
//     {0x29, EINPUT_KEY_CODE::INPUT_KEYBOARD_SEMICOLON},     // ';' key
//     {0x2A, EINPUT_KEY_CODE::INPUT_KEYBOARD_BACKSLASH},     // '\\' key
//     {0x2B, EINPUT_KEY_CODE::INPUT_KEYBOARD_COMMA},         // ',' key
//     {0x2C, EINPUT_KEY_CODE::INPUT_KEYBOARD_SLASH},         // '/' key
//     {0x2D, EINPUT_KEY_CODE::INPUT_KEYBOARD_N},             // 'N' key
//     {0x2E, EINPUT_KEY_CODE::INPUT_KEYBOARD_M},             // 'M' key
//     {0x2F, EINPUT_KEY_CODE::INPUT_KEYBOARD_PERIOD},        // '.' key
//     {0x30, EINPUT_KEY_CODE::INPUT_KEYBOARD_TAB},           // Tab key
//     {0x31, EINPUT_KEY_CODE::INPUT_KEYBOARD_SPACE},         // Space key
//     {0x32, EINPUT_KEY_CODE::INPUT_KEYBOARD_GRAVE_ACCENT},  // '`' key
//     {0x33, EINPUT_KEY_CODE::INPUT_KEYBOARD_BACKSPACE},     // Backspace key
//     {0x35, EINPUT_KEY_CODE::INPUT_KEYBOARD_ESCAPE},        // Escape key
//     // Add more mappings as needed
// };
//     EINPUT_KEY_CODE MapKeyCode(uint16_t nsKeyCode);
// };

// }
// #endif
// #endif
