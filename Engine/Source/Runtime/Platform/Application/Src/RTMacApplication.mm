#include "../Include/RTMacApplication.h"
#include <AppKit/AppKit.h>
#include "Platform/Application/Include/RTApplication.h"
#include "Platform/HAL/Input/Include/InputEnums.h"
#include "Platform/HAL/Window/Include/MACOSX/MacOSXWindow.h"
#include "Platform/HAL/Window/Include/WindowEnums.h"

    void testKeyDown(const ReiToEngine::InputEvent& event) {
    printf("KeyDown: %hu\n", event.inputCode);
    printf("KeyDown: %d\n", event.codepoint);
    if (event.inputCode == ReiToEngine::EINPUT_KEY_CODE::INPUT_KEYBOARD_ESCAPE) {
            ReiToEngine::RTApplication::Instance().app_state.is_running = false;
        }
    }
namespace ReiToEngine {
void RTMacApplication::Initialize(ApplicatonConfig& config) {
    RTApplication::Initialize(config);

    // macOS 特定初始化
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    // 创建主菜单
    NSMenu* mainMenu = [[NSMenu alloc] init];
    [NSApp setMainMenu:mainMenu];

    // 创建应用程序菜单项
    NSMenuItem* appMenuItem = [[NSMenuItem alloc] init];
    [mainMenu addItem:appMenuItem];

    // 创建应用程序菜单
    NSMenu* appMenu = [[NSMenu alloc] init];
    [appMenuItem setSubmenu:appMenu];

    // 添加退出菜单项
    NSString* quitTitle =
        [@"Quit " stringByAppendingString:[[NSProcessInfo processInfo] processName]];
    NSMenuItem* quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
                                                          action:@selector(terminate:)
                                                   keyEquivalent:@"q"];
    [appMenu addItem:quitMenuItem];

    [NSApp activateIgnoringOtherApps:YES];
    printf("mac init\n");
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

    inputSystem_ptr->AddInputCallback(EINPUT_EVENT_TYPE::EVENT_KEY_PRESS,
    testKeyDown);

    printf("mac run\n");
}

void RTMacApplication::Terminate() {
     RTApplication::Terminate();
     [NSApp terminate:nil];
}
}  // namespace ReiToEngine
