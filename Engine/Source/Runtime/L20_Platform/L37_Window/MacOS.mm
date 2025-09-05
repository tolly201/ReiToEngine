#include "Functions.h"
#ifdef RT_SYSTEM_APPLE
#include <AppKit/AppKit.h>
#include "./MACOSX/MacOSXWindow.h"

b8 RT_Platform_MainWindowInitialize(RT_Platform_State* platform_state)
{
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
    return true;
}

void RT_Platform_MainWindowTerminate(RT_Platform_State* platform_state)
{
}

b8 RT_Platform_MainWindowPumpMessage(RT_Platform_State* platform_state){
    NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                        untilDate:[NSDate distantPast]
                                           inMode:NSDefaultRunLoopMode
                                          dequeue:YES];
    if (event) {
        [NSApp sendEvent:event];  // 分发事件
    }
    return true;
}
#endif
