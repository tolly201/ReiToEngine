#include "../Include/RTMacApplication.h"
#include "Platform/Application/Include/RTApplication.h"
#include "Platform/HAL/Window/Include/MACOSX/MacOSXWindow.h"
#include "Platform/HAL/Window/Include/WindowEnums.h"



namespace ReiToEngine {
void RTMacApplication::Initialize() {
    RTApplication::Initialize();
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
}

void RTMacApplication::Tick() {
    RTApplication::Tick();
    NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                        untilDate:[NSDate distantPast]
                                           inMode:NSDefaultRunLoopMode
                                          dequeue:YES];
    if (event) {
        [NSApp sendEvent:event];  // 分发事件
    }
}

void RTMacApplication::Run() {
    RTApplication::Run();
    // [NSApp run];
}

void RTMacApplication::Terminate() {
     RTApplication::Terminate();
     [NSApp terminate:nil];
}
}  // namespace ReiToEngine
