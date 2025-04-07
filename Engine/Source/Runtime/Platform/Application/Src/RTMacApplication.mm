#include "../Include/RTMacApplication.h"
#include "Platform/HAL/Window/Include/MACOSX/MacOSXWindow.h"
#include "Platform/HAL/Window/Include/WindowEnums.h"

void testKeyDown(IWindow* window, EWINDOW_KEYBOARD_KEY key, EWINDOW_KEYBOARD_MOD mods)
{
    printf("KeyDown: %d\n", key);
    if (key == EWINDOW_KEYBOARD_KEY::KB_KEY_ESCAPE)
    {
        window->CloseWindow();
    }
}
void testWindow() {
        // 1. 创建 NSApplication 实例
        MacOSXWindow* window = new MacOSXWindow();
        uint32_t width = 80;
        uint32_t height = 60;
        uint8_t channel = 4;
        uint8_t* buffer = new uint8[
            width * height * channel
        ];
        for(size_t i = 0; i < width * height * channel; ++i)
        {
            buffer[i] = 0;
            if ((i + 1) % 4 == 0) buffer[i] = 255;
        }
        window->AddKeyDownCallback(testKeyDown);
        window->Create("test", width, height);
        window->cocoaView->SetHeight(height);
        window->cocoaView->SetWidth(width);
        window->cocoaView->SetChannel(channel);
        window->cocoaView->SetBuffer(buffer);
        window->ShowWindow();
        window->Update(nullptr, 20,20);
}

namespace ReiToEngine {
void RTMacApplication::Initialize()
{
    printf("BASE_INIT\n");
    RTApplication::Initialize();
    printf("MAC_INIT\n");
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
    NSString* quitTitle = [@"Quit " stringByAppendingString:[[NSProcessInfo processInfo] processName]];
    NSMenuItem* quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
                                                        action:@selector(terminate:)
                                                 keyEquivalent:@"q"];
    [appMenu addItem:quitMenuItem];

    [NSApp activateIgnoringOtherApps:YES];

    testWindow();
    [NSApp run];
}
}
