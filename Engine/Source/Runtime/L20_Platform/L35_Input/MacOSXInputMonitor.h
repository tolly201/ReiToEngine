#ifndef PLATFORM_MACOSX_INPUT_MONITOR_H
#define PLATFORM_MACOSX_INPUT_MONITOR_H
#include "L0_Macro/Include.h"
#include "L20_Platform/L32_Containers/Map.h"
#ifdef RT_SYSTEM_APPLE
#include "InputMonitor.h"
#import <Cocoa/Cocoa.h>

namespace ReiToEngine {
class MacOSXInputMonitor;
}

namespace ReiToEngine {
class MacOSXInputMonitor : public InputMonitor {
  public:
    MacOSXInputMonitor();
    ~MacOSXInputMonitor();
    void ProcessNativeEvent(NSEvent *event);

  protected:
    const ReiToEngine::Map<u16, KEY_CODE_KEYBOARD> keyCodeMap = {
        {0x00, KEY_CODE_KEYBOARD::A},             // 'A' key
        {0x01, KEY_CODE_KEYBOARD::S},             // 'S' key
        {0x02, KEY_CODE_KEYBOARD::D},             // 'D' key
        {0x03, KEY_CODE_KEYBOARD::F},             // 'F' key
        {0x04, KEY_CODE_KEYBOARD::H},             // 'H' key
        {0x05, KEY_CODE_KEYBOARD::G},             // 'G' key
        {0x06, KEY_CODE_KEYBOARD::Z},             // 'Z' key
        {0x07, KEY_CODE_KEYBOARD::X},             // 'X' key
        {0x08, KEY_CODE_KEYBOARD::C},             // 'C' key
        {0x09, KEY_CODE_KEYBOARD::V},             // 'V' key
        {0x0B, KEY_CODE_KEYBOARD::B},             // 'B' key
        {0x0C, KEY_CODE_KEYBOARD::Q},             // 'Q' key
        {0x0D, KEY_CODE_KEYBOARD::W},             // 'W' key
        {0x0E, KEY_CODE_KEYBOARD::E},             // 'E' key
        {0x0F, KEY_CODE_KEYBOARD::R},             // 'R' key
        {0x10, KEY_CODE_KEYBOARD::Y},             // 'Y' key
        {0x11, KEY_CODE_KEYBOARD::T},             // 'T' key
        {0x12, KEY_CODE_KEYBOARD::NUM_1},         // '1' key
        {0x13, KEY_CODE_KEYBOARD::NUM_2},         // '2' key
        {0x14, KEY_CODE_KEYBOARD::NUM_3},         // '3' key
        {0x15, KEY_CODE_KEYBOARD::NUM_4},         // '4' key
        {0x16, KEY_CODE_KEYBOARD::NUM_6},         // '6' key
        {0x17, KEY_CODE_KEYBOARD::NUM_5},         // '5' key
        {0x18, KEY_CODE_KEYBOARD::KP_EQUAL},      // '=' key
        {0x19, KEY_CODE_KEYBOARD::NUM_9},         // '9' key
        {0x1A, KEY_CODE_KEYBOARD::NUM_7},         // '7' key
        {0x1B, KEY_CODE_KEYBOARD::MINUS},         // '-' key
        {0x1C, KEY_CODE_KEYBOARD::NUM_8},         // '8' key
        {0x1D, KEY_CODE_KEYBOARD::NUM_0},         // '0' key
        {0x1E, KEY_CODE_KEYBOARD::RIGHT_BRACKET}, // ']' key
        {0x1F, KEY_CODE_KEYBOARD::O},             // 'O' key
        {0x20, KEY_CODE_KEYBOARD::U},             // 'U' key
        {0x21, KEY_CODE_KEYBOARD::LEFT_BRACKET},  // '[' key
        {0x22, KEY_CODE_KEYBOARD::I},             // 'I' key
        {0x23, KEY_CODE_KEYBOARD::P},             // 'P' key
        {0x24, KEY_CODE_KEYBOARD::LEFT_ALT},      // Return key
        {0x25, KEY_CODE_KEYBOARD::L},             // 'L' key
        {0x26, KEY_CODE_KEYBOARD::J},             // 'J' key
        {0x27, KEY_CODE_KEYBOARD::APOSTROPHE},    // '\'' key
        {0x28, KEY_CODE_KEYBOARD::K},             // 'K' key
        {0x29, KEY_CODE_KEYBOARD::SEMICOLON},     // ';' key
        {0x2A, KEY_CODE_KEYBOARD::BACKSLASH},     // '\\' key
        {0x2B, KEY_CODE_KEYBOARD::COMMA},         // ',' key
        {0x2C, KEY_CODE_KEYBOARD::SLASH},         // '/' key
        {0x2D, KEY_CODE_KEYBOARD::N},             // 'N' key
        {0x2E, KEY_CODE_KEYBOARD::M},             // 'M' key
        {0x2F, KEY_CODE_KEYBOARD::PERIOD},        // '.' key
        {0x30, KEY_CODE_KEYBOARD::TAB},           // Tab key
        {0x31, KEY_CODE_KEYBOARD::SPACE},         // Space key
        {0x32, KEY_CODE_KEYBOARD::GRAVE_ACCENT},  // '`' key
        {0x33, KEY_CODE_KEYBOARD::BACKSPACE},     // Backspace key
        {0x35, KEY_CODE_KEYBOARD::ESCAPE},        // Escape key
        // Add more mappings as needed
    };
    KEY_CODE_KEYBOARD MapKeyCode(uint16_t nsKeyCode);
};

} // namespace ReiToEngine
#endif
#endif
