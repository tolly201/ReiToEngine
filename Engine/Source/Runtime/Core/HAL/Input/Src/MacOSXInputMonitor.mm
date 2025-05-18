#include "Core/MinimalCore.h"
#ifdef RT_SYSTEM_APPLE
#include "../Include/MacOSXInputMonitor.h"
#include <Foundation/Foundation.h>
#include "Core/HAL/Input/Include/InputEnums.h"
#include "Core/HAL/Input/Include/InputMonitor.h"


@implementation NSInputMonitor
- (id)init {
    self = [super init];
    return self;
}

- (void)addMonitor:(ReiToEngine::MacOSXInputMonitor*)monitor {
        _monitor = monitor;
}

- (void)startMonitoring {
    NSEvent* (^handler)(NSEvent*) = ^NSEvent*(NSEvent* event) {
        NSLog(@"local event trigger");
        printf("catch local input event: %ld\n", (long)[event type]);
        printf("%ld\n", NSEventTypeScrollWheel);
        _monitor->InnerConvertNSEvent(event);
      return event;
    };

    // [NSEvent
    //     addGlobalMonitorForEventsMatchingMask:(NSEventMaskKeyDown | NSEventMaskKeyUp |
    //                                            NSEventMaskLeftMouseDown | NSEventMaskLeftMouseUp |
    //                                            NSEventMaskRightMouseDown | NSEventMaskRightMouseUp |
    //                                            NSEventMaskMouseMoved | NSEventMaskScrollWheel |
    //                                            NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp |
    //                                            NSEventMaskFlagsChanged)
    //                                   handler:^(NSEvent* event) {
    //                                     NSLog(@"global event trigger");
    //                                   }];

    [NSEvent
        addLocalMonitorForEventsMatchingMask:(NSEventMaskKeyDown | NSEventMaskKeyUp |
                                              NSEventMaskLeftMouseDown | NSEventMaskLeftMouseUp |
                                              NSEventMaskRightMouseDown | NSEventMaskRightMouseUp |
                                              NSEventMaskMouseMoved | NSEventMaskScrollWheel |
                                              NSEventMaskOtherMouseDown | NSEventMaskOtherMouseUp |
                                              NSEventMaskFlagsChanged)
                                     handler:handler];


}

@end

namespace ReiToEngine {

constexpr EINPUT_MODIFIER operator|(EINPUT_MODIFIER a, EINPUT_MODIFIER b) {
    return static_cast<EINPUT_MODIFIER>(
        static_cast<std::underlying_type_t<EINPUT_MODIFIER>>(a) |
        static_cast<std::underlying_type_t<EINPUT_MODIFIER>>(b)
    );
}

EINPUT_MODIFIER& operator|=(EINPUT_MODIFIER& a, EINPUT_MODIFIER b) {
    a = a | b;
    return a;
}
MacOSXInputMonitor::MacOSXInputMonitor() {
    // 创建并启动事件监听器
    cocoaInputMonitor = [[NSInputMonitor alloc] init];
    [cocoaInputMonitor addMonitor:this];
    printf("macos input monitor created");
    [cocoaInputMonitor startMonitoring];
}

MacOSXInputMonitor::~MacOSXInputMonitor() {
    if (cocoaInputMonitor) {
        [cocoaInputMonitor release];
    }
}

void MacOSXInputMonitor::InnerConvertNSEvent(NSEvent* event) {
    InputEvent inputEvent;

    printf("catch local input event: %ld\n", (long)[event type]);
    printf("%ld\n", NSEventTypeMouseMoved);

    // Initialize the input event
    inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_UNKNOWN;
    inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_UNKNOWN;
    inputEvent.inputCode = EINPUT_KEY_CODE::INPUT_UNKNOWN;
    inputEvent.modifiers = EINPUT_MODIFIER::MOD_NONE;
    inputEvent.codepoint = 0;

    switch ([event type]) {
        // Key down event
        case NSEventTypeKeyDown: {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_KEYBOARD;
            inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_KEY_PRESS;

            // Extract key code
            uint16_t keyCode = [event keyCode];
            inputEvent.inputCode = MapKeyCode(keyCode);

            // Extract codepoint (if available)
            NSString* chars = [event characters];
            if ([chars length] > 0) {
                inputEvent.codepoint = [chars characterAtIndex:0];
            }

            // Handle modifier flags
            NSEventModifierFlags flags = [event modifierFlags];
            inputEvent.modifiers = EINPUT_MODIFIER::MOD_NONE;
            if (flags & NSEventModifierFlagShift) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_SHIFT;
            if (flags & NSEventModifierFlagControl) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_CONTROL;
            if (flags & NSEventModifierFlagOption) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_ALT;
            if (flags & NSEventModifierFlagCommand) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_SUPER;

            break;
        }

        // Key up event
        case NSEventTypeKeyUp: {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_KEYBOARD;
            inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_KEY_RELEASE;

            // Extract key code
            uint16_t keyCode = [event keyCode];
            inputEvent.inputCode = MapKeyCode(keyCode);
            // Handle modifier flags (optional, for consistency)
            NSEventModifierFlags flags = [event modifierFlags];
            inputEvent.modifiers = EINPUT_MODIFIER::MOD_NONE;
            if (flags & NSEventModifierFlagShift) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_SHIFT;
            if (flags & NSEventModifierFlagControl) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_CONTROL;
            if (flags & NSEventModifierFlagOption) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_ALT;
            if (flags & NSEventModifierFlagCommand) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_SUPER;

            break;
        }

        // Modifier key changes (flags changed)
        case NSEventTypeFlagsChanged: {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_KEYBOARD;

            // Determine if this is a press or release for modifier keys
            static NSEventModifierFlags lastFlags = 0;
            NSEventModifierFlags flags = [event modifierFlags];

            if (flags > lastFlags) {
                inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_KEY_PRESS;
            } else {
                inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_KEY_RELEASE;
            }

            // Update last flags
            lastFlags = flags;

            // Map modifier keys
            inputEvent.inputCode = EINPUT_KEY_CODE::INPUT_UNKNOWN; // Modifier keys don't map to a specific key code
            inputEvent.modifiers = EINPUT_MODIFIER::MOD_NONE;
            if (flags & NSEventModifierFlagShift) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_SHIFT;
            if (flags & NSEventModifierFlagControl) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_CONTROL;
            if (flags & NSEventModifierFlagOption) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_ALT;
            if (flags & NSEventModifierFlagCommand) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_SUPER;

            break;
        }

        // Mouse button events
        case NSEventTypeLeftMouseDown:
        {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_MOUSE;
                inputEvent.inputCode = EINPUT_KEY_CODE::INPUT_MOUSE_BUTTON_LEFT;
                inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_POINTER_DOWN;
            break;
        }
        case NSEventTypeLeftMouseUp:
                {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_MOUSE;
                inputEvent.inputCode = EINPUT_KEY_CODE::INPUT_MOUSE_BUTTON_LEFT;
                inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_POINTER_UP;
            break;
        }
        case NSEventTypeRightMouseDown:
                        {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_MOUSE;
                inputEvent.inputCode = EINPUT_KEY_CODE::INPUT_MOUSE_BUTTON_RIGHT;
                inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_POINTER_DOWN;
            break;
        }
        case NSEventTypeRightMouseUp:
                        {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_MOUSE;
                inputEvent.inputCode = EINPUT_KEY_CODE::INPUT_MOUSE_BUTTON_RIGHT;
                inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_POINTER_UP;
            break;
        }
        case NSEventTypeOtherMouseDown:
        case NSEventTypeOtherMouseUp:
        // Mouse movement
        case NSEventTypeMouseMoved:
        {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_MOUSE;
            inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_POINTER_MOVE;
            NSPoint location = [event locationInWindow];
            inputEvent.positionX = static_cast<float>(location.x); // Also set positionX
            inputEvent.positionY = static_cast<float>(location.y); // Also set positionY
            break;
        }

        case NSEventTypeLeftMouseDragged:
        case NSEventTypeRightMouseDragged:
        case NSEventTypeOtherMouseDragged:
        // Scroll wheel
        case NSEventTypeScrollWheel: {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_MOUSE;
            inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_SCROLL;

            // Extract scroll delta
            CGFloat deltaX = [event scrollingDeltaX];
            CGFloat deltaY = [event scrollingDeltaY];
            inputEvent.scrollDeltaX = static_cast<float>(deltaX);
            inputEvent.scrollDeltaY = static_cast<float>(deltaY);
            break;
        }

        // Unknown event type
        default: {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_UNKNOWN;
            inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_UNKNOWN;
            break;
        }
    }

    // Invoke the callback if active
    if (isActive && inputCallback != nullptr) {
        inputCallback(inputEvent);
    }
}
EINPUT_KEY_CODE MacOSXInputMonitor::MapKeyCode(uint16_t nsKeyCode) {
    auto it = keyCodeMap.find(nsKeyCode);
    if (it != keyCodeMap.end()) {
        return it->second;
    }
    return EINPUT_KEY_CODE::INPUT_UNKNOWN; // Default fallback for unmapped keys
}

}  // namespace ReiToEngine
#endif
