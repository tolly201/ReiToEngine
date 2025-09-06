#include "PlatformFunctions.h"
#ifdef RT_SYSTEM_APPLE
#include <AppKit/AppKit.h>
#include "L20_Platform/L37_Window/MACOSX/MacOSXWindow.h"
#include <sys/mman.h> // For mmap, munmap
#include <unistd.h>  // For sysconf, _SC_PAGE_SIZE
#include <cstdio>

b8 RT_Platform_Initialize(RT_Platform_State*& platform_state)
{
    platform_state = new RT_Platform_State();
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

void RT_Platform_Terminate(RT_Platform_State*& platform_state)
{
    delete platform_state;
}

b8 RT_Platform_PumpMessage(RT_Platform_State*& platform_state){
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
