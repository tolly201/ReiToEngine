#import "../../Include/MACOSX/MacOSXWindow.h"
#include <cstdio>
#include "Platform/HAL/Window/Include/WindowEnums.h"
#include <iostream>
@implementation OSXWindow

- (id)initWithContentRect:(NSRect)contentRect
                styleMask:(NSWindowStyleMask)windowStyle
                  backing:(NSBackingStoreType)bufferingType
                    defer:(BOOL)deferCreation
{
    self = [super initWithContentRect:contentRect
                            styleMask:windowStyle
                              backing:bufferingType
                                defer:deferCreation];
    if (self) {
        NSLog(@"Inner Window created successfully.");
        [self setContentView:childContentView];
        [self setDelegate:self]; // Make OSXWindow itself the delegate
    }
    return self;
}


- (void)removeWindowData
{
    // You can add code here to remove any associated window data if needed.
    // For example, if you have a custom data structure linked to the window.
}

- (BOOL)windowShouldClose:(id)sender
{
    return YES; // Allow window to close
}

- (void)windowWillClose:(NSNotification *)notification
{
    // Notification sent before the window is closed.
    // You can perform actions before window closure if needed.
}

- (void)windowDidResize:(NSNotification *)notification
{
    // Notification sent after the window is resized.
    // You can adjust UI elements or trigger layout updates here.
}

- (void)windowDidMove:(NSNotification *)notification
{
    // Notification sent after the window is moved.
    // Useful for scenarios where window position is important.
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
    // Notification sent when the window becomes the key window.
    // Key window is the window that receives keyboard input.
}

- (void)windowDidResignKey:(NSNotification *)notification
{
    // Notification sent when the window resigns being the key window.
}

- (void)dealloc
{
    [childContentView release];
    [super dealloc];
}

@end

IWindow* MacOSXWindow::Create(const char* title, uint32_t _width, uint32_t _height)
{
    std::cout << "create window\n";
    NSRect contentRect = NSMakeRect(0, 0, _width, _height);
    NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable;
    cocoaWindow = [[OSXWindow alloc] initWithContentRect:contentRect
                                                        styleMask:styleMask
                                                          backing:NSBackingStoreBuffered
                                                            defer:NO];
    cocoaView = new MacOSXView();
    OSXView* instance = [[OSXView alloc] initWithFrame:contentRect]; // 初始化 OSXView 实例并设置 frame
    [cocoaWindow setContentView:instance]; // 设置 content view
    cocoaView->SetViewInstance(instance);

    [instance bindWindow:this];

    if (cocoaWindow) {
    std::cout << "create window cocoa\n";
    printf("size:%d, %d\n", _width, _height);
        [cocoaWindow setTitle:[NSString stringWithUTF8String:title]];
        // title = strdup(title); // Using strdup for char* title, remember to free in destructor
        // width = _width;
        // height = _height;
        BOOL success = [cocoaWindow makeFirstResponder:instance];
        printf("first reson? %d", success);
        return this;
    } else {
        return nullptr;
    }
}

MacOSXWindow::~MacOSXWindow()
{
    if (title) {
        free(title); // Free the allocated char* for title
    }
    if (cocoaWindow) {
        [cocoaWindow close];
        [cocoaWindow release]; // Release the allocated OSXWindow
        cocoaWindow = nil;
    }
}


void MacOSXWindow::SetTitle(const char* title)
{
    if (cocoaWindow) {
        [cocoaWindow setTitle:[NSString stringWithUTF8String:title]];
        if (this->title) free(this->title); // Free previously allocated title
        this->title = strdup(title);       // Allocate and copy new title
    }
}

char* MacOSXWindow::GetTitle() const
{
    return title;
}

void MacOSXWindow::SetSize(uint32_t width, uint32_t height)
{
    if (cocoaWindow) {
        NSRect contentRect = NSMakeRect(0, 0, width, height);
        [cocoaWindow setContentSize:contentRect.size];
        this->width = width;
        this->height = height;
    }
}

uint32_t MacOSXWindow::GetWidth() const
{
    return width;
}

uint32_t MacOSXWindow::GetHeight() const
{
    return height;
}

void MacOSXWindow::ShowWindow()
{
    std::cout << "show it \n";
        if (cocoaWindow) {
            std::cout << "show\n";
            NSRect windowFrame = [cocoaWindow frame]; // 获取窗口 Frame
            NSLog(@"Window Frame: x=%f, y=%f, width=%f, height=%f", windowFrame.origin.x, windowFrame.origin.y, windowFrame.size.width, windowFrame.size.height); // 打印 Frame 信息
            [cocoaWindow makeKeyAndOrderFront:nil];
        }
}

void MacOSXWindow::HideWindow()
{
    if (cocoaWindow) {
        [cocoaWindow orderOut:nil];
    }
}

void MacOSXWindow::CloseWindow()
{
    if (cocoaWindow) {
        [cocoaWindow close];
        [cocoaWindow release];
        cocoaWindow = nil;
        delete this; // Self-delete in CloseWindow, consider alternatives for memory management
    }
}

void MacOSXWindow::ProcessEvents()
{
    // macOS event processing is largely handled by the system.
    // You might add custom event polling or handling here if needed.
}

void MacOSXWindow::Update(const void* buffer, uint32_t width, uint32_t height)
{
    // [[NSApplication sharedApplication] run];
        // Placeholder for buffer update and display logic.
        // You will need to implement the platform-specific rendering here,
        // potentially using Core Graphics, Metal, or OpenGL with the buffer data.

        cocoaView->Draw();
        NSLog(@"MacOSXWindow::Update - Buffer display logic needs implementation.");
        [NSApp run];
}

// --- 实现 Add 回调方法 ---
void MacOSXWindow::AddKeyDownCallback(KeyDownCallback callback) { m_keyDownCallbacks.push_back(std::move(callback)); }
void MacOSXWindow::AddKeyUpCallback(KeyUpCallback callback) { m_keyUpCallbacks.push_back(std::move(callback)); }
void MacOSXWindow::AddCharInputCallback(CharInputCallback callback) { m_charInputCallbacks.push_back(std::move(callback)); }
void MacOSXWindow::AddMouseButtonDownCallback(MouseButtonDownCallback callback) { m_mouseButtonDownCallbacks.push_back(std::move(callback)); }
void MacOSXWindow::AddMouseButtonUpCallback(MouseButtonUpCallback callback) { m_mouseButtonUpCallbacks.push_back(std::move(callback)); }
void MacOSXWindow::AddMouseMoveCallback(MouseMoveCallback callback) { m_mouseMoveCallbacks.push_back(std::move(callback)); }
void MacOSXWindow::AddMouseScrollCallback(MouseScrollCallback callback) { m_mouseScrollCallbacks.push_back(std::move(callback)); }
void MacOSXWindow::AddMouseEnterCallback(MouseEnterCallback callback) { m_mouseEnterCallbacks.push_back(std::move(callback)); }
void MacOSXWindow::AddWindowFocusCallback(WindowFocusCallback callback) { m_windowFocusCallbacks.push_back(std::move(callback)); }

// --- 实现 Clear 回调方法 ---
void MacOSXWindow::ClearKeyDownCallbacks() { m_keyDownCallbacks.clear(); }
void MacOSXWindow::ClearKeyUpCallbacks() { m_keyUpCallbacks.clear(); }
void MacOSXWindow::ClearCharInputCallbacks() { m_charInputCallbacks.clear(); }
void MacOSXWindow::ClearMouseButtonDownCallbacks() { m_mouseButtonDownCallbacks.clear(); }
void MacOSXWindow::ClearMouseButtonUpCallbacks() { m_mouseButtonUpCallbacks.clear(); }
void MacOSXWindow::ClearMouseMoveCallbacks() { m_mouseMoveCallbacks.clear(); }
void MacOSXWindow::ClearMouseScrollCallbacks() { m_mouseScrollCallbacks.clear(); }
void MacOSXWindow::ClearMouseEnterCallbacks() { m_mouseEnterCallbacks.clear(); }
void MacOSXWindow::ClearWindowFocusCallbacks() { m_windowFocusCallbacks.clear(); }
void MacOSXWindow::ClearAllCallbacks() {
    ClearKeyDownCallbacks();
    ClearKeyUpCallbacks();
    ClearCharInputCallbacks();
    ClearMouseButtonDownCallbacks();
    ClearMouseButtonUpCallbacks();
    ClearMouseMoveCallbacks();
    ClearMouseScrollCallbacks();
    ClearMouseEnterCallbacks();
    ClearWindowFocusCallbacks();
}

void MacOSXWindow::HandleKeyDown(ushort key, int mods)
{
    EWINDOW_KEYBOARD_KEY key_map = ConvertMacKeyCodeToMfbKey(key);
    EWINDOW_KEYBOARD_MOD key_mod = ConvertMacModifierFlagsToMfbMods(mods);
    printf("Handler key_map:%d", key_map);
    printf("callback count:%d", m_keyDownCallbacks.size());
    for (const auto& callback : m_keyDownCallbacks) {
        if (callback) { // 检查 std::function 是否有效
            callback(this, key_map, key_mod);
        }
    }
}
void MacOSXWindow::HandleKeyUp(EWINDOW_KEYBOARD_KEY key, EWINDOW_KEYBOARD_MOD mods)
{
     for (const auto& callback : m_keyUpCallbacks) {
        if (callback) {
            callback(this, key, mods);
        }
    }
}
void MacOSXWindow::HandleCharInput(unsigned int codepoint)
{
     for (const auto& callback : m_charInputCallbacks) {
        if (callback) {
            callback(this, codepoint);
        }
    }
}
void MacOSXWindow::HandleMouseButtonDown(EWINDOW_MOUSE_BTN button, int x, int y, EWINDOW_KEYBOARD_MOD mods)
{
    for (const auto& callback : m_mouseButtonDownCallbacks) {
        if (callback) {
            callback(this, button, x, y, mods);
        }
    }
}
void MacOSXWindow::HandleMouseButtonUp(EWINDOW_MOUSE_BTN button, int x, int y, EWINDOW_KEYBOARD_MOD mods)
{
    for (const auto& callback : m_mouseButtonUpCallbacks) {
        if (callback) {
            callback(this, button, x, y, mods);
        }
    }
}
void MacOSXWindow::HandleMouseMove(int x, int y)
{
    for (const auto& callback : m_mouseMoveCallbacks) {
        if (callback) {
            callback(this, x, y);
        }
    }
}
void MacOSXWindow::HandleMouseScroll(float scrollX, float scrollY)
{
    for (const auto& callback : m_mouseScrollCallbacks) {
        if (callback) {
            callback(this, scrollX, scrollY);
        }
    }
}
void MacOSXWindow::HandleMouseEnter(bool entered)
{
    for (const auto& callback : m_mouseEnterCallbacks) {
        if (callback) {
            callback(this, entered);
        }
    }
}
void MacOSXWindow::HandleWindowFocus(bool focused)
{
     for (const auto& callback : m_windowFocusCallbacks) {
        if (callback) {
            callback(this, focused);
        }
    }
}

// --- 实现辅助转换函数 ---
EWINDOW_KEYBOARD_KEY MacOSXWindow::ConvertMacKeyCodeToMfbKey(unsigned short macKeyCode) {
    // 这里需要一个映射表，将 macOS 的虚拟键码转换为 mfb_key
    // 示例:
    printf("actual accepted keycode: %d\n", macKeyCode);
    switch (macKeyCode) {
        case 0: return KB_KEY_A;
        case 1: return KB_KEY_S;
        case 2: return KB_KEY_D;
        // ... 其他键 ...
        case 49: return KB_KEY_SPACE;
        case 53: return KB_KEY_ESCAPE;
        default: return KB_KEY_UNKNOWN;
    }
}

EWINDOW_KEYBOARD_MOD MacOSXWindow::ConvertMacModifierFlagsToMfbMods(NSUInteger macFlags) {
    EWINDOW_KEYBOARD_MOD mods = EWINDOW_KEYBOARD_MOD::KB_MOD_ALT;

    return mods;
}
