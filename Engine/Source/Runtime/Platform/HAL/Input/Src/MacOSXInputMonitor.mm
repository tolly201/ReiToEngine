#include "../Include/MacOSXInputMonitor.h"
#include <Foundation/Foundation.h>
#include "Platform/HAL/Input/Include/InputEnums.h"
#include "Platform/HAL/Input/Include/InputMonitor.h"


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
    switch ([event type]) {
        case NSEventTypeKeyDown:
        {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_KEYBOARD;
            uint16_t keyCode = [event keyCode];
            inputEvent.inputCode = static_cast<EINPUT_KEY_CODE>(keyCode);

            // Handle modifier flags
            NSEventModifierFlags flags = [event modifierFlags];
            inputEvent.modifiers = EINPUT_MODIFIER::MOD_NONE;
            if (flags & NSEventModifierFlagShift) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_SHIFT;
            if (flags & NSEventModifierFlagControl) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_CONTROL;
            if (flags & NSEventModifierFlagOption) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_ALT;
            if (flags & NSEventModifierFlagCommand) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_SUPER;

            inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_KEY_PRESS;
            break;
        }
        case NSEventTypeKeyUp:
        case NSEventTypeFlagsChanged:
        {
            inputEvent.deviceType = EINPUT_DEVICE_TYPE::DEVICE_KEYBOARD;
            uint16_t keyCode = [event keyCode];
            inputEvent.inputCode = static_cast<EINPUT_KEY_CODE>(keyCode);

            // Handle modifier flags
            NSEventModifierFlags flags = [event modifierFlags];
            inputEvent.modifiers = EINPUT_MODIFIER::MOD_NONE;
            if (flags & NSEventModifierFlagShift) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_SHIFT;
            if (flags & NSEventModifierFlagControl) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_CONTROL;
            if (flags & NSEventModifierFlagOption) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_ALT;
            if (flags & NSEventModifierFlagCommand) inputEvent.modifiers |= EINPUT_MODIFIER::MOD_KEYBOARD_SUPER;

            if ([event type] == NSEventTypeKeyDown) {
                inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_KEY_PRESS;
                NSString* chars = [event characters];
                if ([chars length] > 0) {
                    inputEvent.codepoint = [chars characterAtIndex:0];
                }
            }
            else if ([event type] == NSEventTypeKeyUp) {
                inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_KEY_RELEASE;
            }
            else if ([event type] == NSEventTypeFlagsChanged) {
                // Determine if this is a press or release for modifier keys
                static NSEventModifierFlags lastFlags = 0;
                if (flags > lastFlags) {
                    inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_KEY_PRESS;
                } else {
                    inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_KEY_RELEASE;
                }
                lastFlags = flags;
            }
            break;
        }

        case NSEventTypeLeftMouseDown:
        case NSEventTypeLeftMouseUp:
        case NSEventTypeRightMouseDown:
        case NSEventTypeRightMouseUp:
        case NSEventTypeMouseMoved:
        case NSEventTypeScrollWheel:
        case NSEventTypeLeftMouseDragged:
        case NSEventTypeRightMouseDragged:
            // Handle mouse events in a separate method
            break;

        default:
            // All other event types
            inputEvent.eventType = EINPUT_EVENT_TYPE::EVENT_UNKNOWN;
            break;
    }

    if (isActive && inputCallback != nullptr) {
        inputCallback(inputEvent);
    }
}

}  // namespace ReiToEngine
