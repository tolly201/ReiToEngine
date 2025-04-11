#ifndef PLATFORM_HAL_WINDOW_MACOSX_WINDOW_H
#define PLATFORM_HAL_WINDOW_MACOSX_WINDOW_H

#import <Cocoa/Cocoa.h>
#include "Platform/HAL/Window/Include/WindowEnums.h"
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
    IWindow* Create(const char* title, uint32_t width, uint32_t height) override; // 返回类型为 IWindow*
    void SetTitle(const char* title) override;
    char* GetTitle() const override;
    void SetSize(uint32_t width, uint32_t height) override;
    uint32_t GetWidth() const override;
    uint32_t GetHeight() const override;
    void ShowWindow() override;
    void HideWindow() override;
    void CloseWindow() override;
    void ProcessEvents() override;
    void Update(const void* buffer, uint32_t width, uint32_t height) override;

    MacOSXView* cocoaView;
protected:
private:
    OSXWindow*  cocoaWindow; // 假设你使用现有的 OSXWindow (NSWindow 子类)
};
#endif
