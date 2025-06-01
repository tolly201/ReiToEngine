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
b8 RTMacApplication::Initialize(IGame* game) {
    return RTApplication::Initialize(game);
}

void RTMacApplication::Tick() {

}

b8 RTMacApplication::Run() {
    return RTApplication::Run();

    // inputSystem_ptr->AddInputCallback(EINPUT_EVENT_TYPE::EVENT_KEY_PRESS,
    // testKeyDown);
}

void RTMacApplication::Terminate() {
     [NSApp terminate:nil];
     RTApplication::Terminate();
}
}  // namespace ReiToEngine
#endif
