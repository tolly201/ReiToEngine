#include "Core/MinimalCore.h"
#ifdef RT_SYSTEM_APPLE
#include "../Include/RTMacApplication.h"
#include <AppKit/AppKit.h>
#include "Platform/Application/Include/RTApplication.h"
#include "Core/HAL/Input/Include/InputEnums.h"
#include "Core/HAL/Window/Include/MACOSX/MacOSXWindow.h"
#include "Core/HAL/Window/Include/WindowEnums.h"

    void testKeyDown(const ReiToEngine::InputEvent& event) {
    printf("KeyDown: %hu\n", event.inputCode);
    printf("KeyDown: %d\n", event.codepoint);
    if (event.inputCode == ReiToEngine::EINPUT_KEY_CODE::INPUT_KEYBOARD_ESCAPE) {
            ReiToEngine::RTApplication::Instance().GetApplicationState().is_running = false;
        }
    }
namespace ReiToEngine {
void RTMacApplication::Initialize(ApplicatonConfig& config) {
    RTApplication::Initialize(config);
}

void RTMacApplication::Tick() {
    NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                        untilDate:[NSDate distantPast]
                                           inMode:NSDefaultRunLoopMode
                                          dequeue:YES];
    if (event) {
        [NSApp sendEvent:event];  // 分发事件
    }
    RTApplication::Tick();
}

void RTMacApplication::Run() {
    RTApplication::Run();

    // inputSystem_ptr->AddInputCallback(EINPUT_EVENT_TYPE::EVENT_KEY_PRESS,
    // testKeyDown);

    printf("mac run\n");
}

void RTMacApplication::Terminate() {
     RTApplication::Terminate();
     [NSApp terminate:nil];
}
}  // namespace ReiToEngine
#endif
