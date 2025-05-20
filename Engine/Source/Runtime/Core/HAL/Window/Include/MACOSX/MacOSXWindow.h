#ifndef PLATFORM_HAL_WINDOW_MACOSX_WINDOW_H
#define PLATFORM_HAL_WINDOW_MACOSX_WINDOW_H
#include "Core/MinimalCore.h"
#ifdef RT_SYSTEM_APPLE
#import <Cocoa/Cocoa.h>
#include "Core/HAL/Window/Include/WindowEnums.h"
#include "../IWindow.h"
#include "MacOSXView.h"
#include <cstdint>
@interface OSXWindow : NSWindow<NSWindowDelegate>
{
    NSView              *childContentView;
}

- (id)initWithContentRect:(NSRect)contentRect
                styleMask:(NSWindowStyleMask)windowStyle
                  backing:(NSBackingStoreType)bufferingType
                    defer:(BOOL)deferCreation
               ;

- (void) removeWindowData;
@end

class RTENGINE_API MacOSXWindow : public IWindow // 正确继承非模板 IWindow
{
public:
    MacOSXWindow() = default;
    ~MacOSXWindow();

    // 实现 IWindow 的纯虚函数
    IWindow* Create(const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y) override; // 返回类型为 IWindow*
    void SetTitle(const char* title) override;
    char* GetTitle() const override;
    void SetSize(u32 width, u32 height) override;
    u32 GetWidth() const override;
    u32 GetHeight() const override;
    void ShowWindow() override;
    void HideWindow() override;
    void CloseWindow() override;
    void ProcessEvents() override;
    void Update(const u8* buffer, u32 width, u32 height) override;

    MacOSXView* cocoaView;
protected:
private:
    OSXWindow*  cocoaWindow; // 假设你使用现有的 OSXWindow (NSWindow 子类)
};
#endif
#endif
