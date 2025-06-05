#include "L0_Macro/Include.h"
#ifdef RT_SYSTEM_APPLE
#include "RTMacApplication.h"
#include <AppKit/AppKit.h>
#include "RTApplication.h"
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
