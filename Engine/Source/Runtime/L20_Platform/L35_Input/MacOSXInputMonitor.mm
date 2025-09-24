#include "L0_Macro/Include.h"
#ifdef RT_SYSTEM_APPLE
#include <Foundation/Foundation.h>

#include "MacOSXInputMonitor.h"

namespace ReiToEngine {
MacOSXInputMonitor::MacOSXInputMonitor() {
}

MacOSXInputMonitor::~MacOSXInputMonitor() {
}

void MacOSXInputMonitor::ProcessNativeEvent(NSEvent* event) {
    InputEvent inputEvent;

    printf("catch local input event: %ld\n", (long)[event type]);
    printf("%ld\n", NSEventTypeMouseMoved);

    // Initialize the input event
    inputEvent.deviceType = INPUT_DEVICE_TYPE::UNKNOWN;
    inputEvent.eventType  = SYSTEM_EVENT_CODE::UNKNOWN;
    inputEvent.modifiers  = KEY_CODE_MODIFIER::NONE;

    switch ([event type]) {
    // Key down event
    case NSEventTypeKeyDown: {
        inputEvent.deviceType = INPUT_DEVICE_TYPE::KEYBOARD;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::KEY_PRESS;

        // Extract key code
        uint16_t keyCode                 = [event keyCode];
        inputEvent.data.keyboard.keyCode = MapKeyCode(keyCode);

        // Extract codepoint (if available)
        NSString* chars = [event characters];
        if ([chars length] > 0) {
        }

        // Handle modifier flags
        NSEventModifierFlags flags = [event modifierFlags];
        inputEvent.modifiers       = KEY_CODE_MODIFIER::NONE;
        if (flags & NSEventModifierFlagShift) inputEvent.modifiers |= KEY_CODE_MODIFIER::SHIFT;
        if (flags & NSEventModifierFlagControl) inputEvent.modifiers |= KEY_CODE_MODIFIER::CONTROL;
        if (flags & NSEventModifierFlagOption) inputEvent.modifiers |= KEY_CODE_MODIFIER::ALT;
        if (flags & NSEventModifierFlagCommand) inputEvent.modifiers |= KEY_CODE_MODIFIER::SUPER;

        ProcessKey(inputEvent, true);
        break;
    }

    // Key up event
    case NSEventTypeKeyUp: {
        inputEvent.deviceType = INPUT_DEVICE_TYPE::KEYBOARD;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::KEY_RELEASE;

        // Extract key code
        uint16_t keyCode                 = [event keyCode];
        inputEvent.data.keyboard.keyCode = MapKeyCode(keyCode);
        // Handle modifier flags (optional, for consistency)
        NSEventModifierFlags flags = [event modifierFlags];
        inputEvent.modifiers       = KEY_CODE_MODIFIER::NONE;
        if (flags & NSEventModifierFlagShift) inputEvent.modifiers |= KEY_CODE_MODIFIER::SHIFT;
        if (flags & NSEventModifierFlagControl) inputEvent.modifiers |= KEY_CODE_MODIFIER::CONTROL;
        if (flags & NSEventModifierFlagOption) inputEvent.modifiers |= KEY_CODE_MODIFIER::ALT;
        if (flags & NSEventModifierFlagCommand) inputEvent.modifiers |= KEY_CODE_MODIFIER::SUPER;

        ProcessKey(inputEvent, false);
        break;
    }

    // Modifier key changes (flags changed)
    case NSEventTypeFlagsChanged: {
        inputEvent.deviceType = INPUT_DEVICE_TYPE::KEYBOARD;

        // Determine if this is a press or release for modifier keys
        static NSEventModifierFlags lastFlags = 0;
        NSEventModifierFlags        flags     = [event modifierFlags];

        if (flags > lastFlags) {
            inputEvent.eventType = SYSTEM_EVENT_CODE::KEY_PRESS;
        } else {
            inputEvent.eventType = SYSTEM_EVENT_CODE::KEY_RELEASE;
        }

        // Update last flags
        lastFlags = flags;

        // Map modifier keys
        inputEvent.modifiers = KEY_CODE_MODIFIER::NONE;
        if (flags & NSEventModifierFlagShift) inputEvent.modifiers |= KEY_CODE_MODIFIER::SHIFT;
        if (flags & NSEventModifierFlagControl) inputEvent.modifiers |= KEY_CODE_MODIFIER::CONTROL;
        if (flags & NSEventModifierFlagOption) inputEvent.modifiers |= KEY_CODE_MODIFIER::ALT;
        if (flags & NSEventModifierFlagCommand) inputEvent.modifiers |= KEY_CODE_MODIFIER::SUPER;

        break;
    }

    // Mouse button events
    case NSEventTypeLeftMouseDown: {
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_BUTTON_PRESS;

        ProcessMouseButton(KEY_CODE_MOUSE::BUTTON_LEFT, true);
        break;
    }
    case NSEventTypeLeftMouseUp: {
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_BUTTON_RELEASE;
        ProcessMouseButton(KEY_CODE_MOUSE::BUTTON_LEFT, false);

        break;
    }
    case NSEventTypeRightMouseDown: {
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_BUTTON_PRESS;
        ProcessMouseButton(KEY_CODE_MOUSE::BUTTON_RIGHT, true);

        break;
    }
    case NSEventTypeRightMouseUp: {
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_BUTTON_RELEASE;
        ProcessMouseButton(KEY_CODE_MOUSE::BUTTON_RIGHT, false);

        break;
    }
    case NSEventTypeOtherMouseDown:
    case NSEventTypeOtherMouseUp:
    // Mouse movement
    case NSEventTypeMouseMoved: {
        inputEvent.deviceType           = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType            = SYSTEM_EVENT_CODE::MOUSE_MOVE;
        NSPoint location                = [event locationInWindow];
        inputEvent.data.mouse.positionX = static_cast<float>(location.x); // Also set positionX
        inputEvent.data.mouse.positionY = static_cast<float>(location.y); // Also set positionY

        ProcessMouseMove(inputEvent.data.mouse.positionX, inputEvent.data.mouse.positionY);
        break;
    }

    case NSEventTypeLeftMouseDragged:
    case NSEventTypeRightMouseDragged:
    case NSEventTypeOtherMouseDragged:
    // Scroll wheel
    case NSEventTypeScrollWheel: {
        inputEvent.deviceType = INPUT_DEVICE_TYPE::MOUSE;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::MOUSE_WHEEL;

        // Extract scroll delta
        CGFloat deltaX                     = [event scrollingDeltaX];
        CGFloat deltaY                     = [event scrollingDeltaY];
        inputEvent.data.mouse.scrollDeltaX = static_cast<float>(deltaX);
        inputEvent.data.mouse.scrollDeltaY = static_cast<float>(deltaY);
        ProcessMouseScroll(inputEvent.data.mouse.scrollDeltaX, inputEvent.data.mouse.scrollDeltaY);
        break;
    }

    // Unknown event type
    default: {
        inputEvent.deviceType = INPUT_DEVICE_TYPE::UNKNOWN;
        inputEvent.eventType  = SYSTEM_EVENT_CODE::UNKNOWN;
        break;
    }
    }

    // Invoke the callback if active
    // if (isActive && inputCallback != nullptr) {
    //     inputCallback(inputEvent);
    // }
}
KEY_CODE_KEYBOARD MacOSXInputMonitor::MapKeyCode(uint16_t nsKeyCode) {
    auto it = keyCodeMap.find(nsKeyCode);
    if (it != keyCodeMap.end()) {
        return it->second;
    }
    return KEY_CODE_KEYBOARD::UNKNOWN; // Default fallback for unmapped keys
}

} // namespace ReiToEngine
#endif
