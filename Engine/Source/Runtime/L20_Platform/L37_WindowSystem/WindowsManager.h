#ifndef PLATFORM_INCLUDE_WINDOWS_MANAGER_H
#define PLATFORM_INCLUDE_WINDOWS_MANAGER_H
#include "L20_Platform/L37_Window/IWindow.h"
#include "L20_Platform/L23_SingletonFactory/RuntimeSingleton.h"
#include "L20_Platform/L24_Containers/List.h"

#include "L20_Platform/L37_Window/IWindow.h"
#include "L20_Platform/L37_Window/IWindowView.h"

#ifdef RT_SYSTEM_APPLE
#include "L20_Platform/L37_Window/MACOSX/MacOSXView.h"
#include "L20_Platform/L37_Window/MACOSX/MacOSXWindow.h"
using PlatformWindow = MacOSXWindow;
#endif

#ifdef RT_SYSTEM_WINDOWS
#include "L20_Platform/L37_Window/Windows/WindowsWindow.h"
using PlatformWindow = WindowsWindow;
#endif

namespace ReiToEngine{
class WindowsManager : public Runtime_Singleton<WindowsManager>
{
public:
    WindowsManager();
    b8 Initialize() override;
    b8 Terminate() override;
    b8 Tick(f64) override;
    u32 RTCreateWindow(u32 width = 100, u32 height = 100, u32 pos_x = 100, u32 pos_y = 100, uint8_t channel = 4);
    void PassViewData(uint8_t* _data, size_t _size, size_t _width, size_t _height);
    PlatformWindow* GetWindow(u32 index);
private:
    ReiToEngine::List<PlatformWindow*> windows;
    uint8_t* data = nullptr;
    [[maybe_unused]]size_t size = 0;
    size_t width = 0;
    size_t height = 0;
};
}
#endif
