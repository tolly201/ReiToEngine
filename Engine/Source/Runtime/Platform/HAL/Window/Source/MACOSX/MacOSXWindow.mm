#import "../../Include/MACOSX/MacOSXWindow.h"
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

    if (cocoaWindow) {
    std::cout << "create window cocoa\n";
    printf("size:%d, %d\n", _width, _height);
        [cocoaWindow setTitle:[NSString stringWithUTF8String:title]];
        // title = strdup(title); // Using strdup for char* title, remember to free in destructor
        // width = _width;
        // height = _height;
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
