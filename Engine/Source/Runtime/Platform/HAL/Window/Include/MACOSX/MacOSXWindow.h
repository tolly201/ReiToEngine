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

    void AddKeyDownCallback(KeyDownCallback callback) override;
    void AddKeyUpCallback(KeyUpCallback callback) override;
    void AddCharInputCallback(CharInputCallback callback) override;
    void AddMouseButtonDownCallback(MouseButtonDownCallback callback) override;
    void AddMouseButtonUpCallback(MouseButtonUpCallback callback) override;
    void AddMouseMoveCallback(MouseMoveCallback callback) override;
    void AddMouseScrollCallback(MouseScrollCallback callback) override;
    void AddMouseEnterCallback(MouseEnterCallback callback) override;
    void AddWindowFocusCallback(WindowFocusCallback callback) override;

    void ClearKeyDownCallbacks() override;
    void ClearKeyUpCallbacks() override;
    void ClearCharInputCallbacks() override;
    void ClearMouseButtonDownCallbacks() override;
    void ClearMouseButtonUpCallbacks() override;
    void ClearMouseMoveCallbacks() override;
    void ClearMouseScrollCallbacks() override;
    void ClearMouseEnterCallbacks() override;
    void ClearWindowFocusCallbacks() override;
    void ClearAllCallbacks() override;

    void HandleKeyDown(ushort key, int mods);
    void HandleKeyUp(EWINDOW_KEYBOARD_KEY key, EWINDOW_KEYBOARD_MOD mods);
    void HandleCharInput(unsigned int codepoint);
    void HandleMouseButtonDown(EWINDOW_MOUSE_BTN button, int x, int y, EWINDOW_KEYBOARD_MOD mods);
    void HandleMouseButtonUp(EWINDOW_MOUSE_BTN button, int x, int y, EWINDOW_KEYBOARD_MOD mods);
    void HandleMouseMove(int x, int y);
    void HandleMouseScroll(float scrollX, float scrollY);
    void HandleMouseEnter(bool entered);
    void HandleWindowFocus(bool focused);

    MacOSXView* cocoaView;
protected:
private:
    OSXWindow*  cocoaWindow; // 假设你使用现有的 OSXWindow (NSWindow 子类)

    std::vector<KeyDownCallback> m_keyDownCallbacks;
    std::vector<KeyUpCallback> m_keyUpCallbacks;
    std::vector<CharInputCallback> m_charInputCallbacks;
    std::vector<MouseButtonDownCallback> m_mouseButtonDownCallbacks;
    std::vector<MouseButtonUpCallback> m_mouseButtonUpCallbacks;
    std::vector<MouseMoveCallback> m_mouseMoveCallbacks;
    std::vector<MouseScrollCallback> m_mouseScrollCallbacks;
    std::vector<MouseEnterCallback> m_mouseEnterCallbacks;
    std::vector<WindowFocusCallback> m_windowFocusCallbacks;

    // 辅助函数: 将 Mac 键码/修饰符转换为引擎格式 (需要在 .mm 中实现)
    EWINDOW_KEYBOARD_KEY ConvertMacKeyCodeToMfbKey(unsigned short macKeyCode);
    EWINDOW_KEYBOARD_MOD ConvertMacModifierFlagsToMfbMods(NSUInteger macFlags);
};
#endif
