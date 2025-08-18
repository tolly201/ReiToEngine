#include "WindowsManager.h"
#include <cstring>
#include "L20_Platform/L37_Window/Functions.h"
namespace ReiToEngine {
WindowsManager::WindowsManager() = default;

b8 WindowsManager::Initialize() {
    return true;
}
b8 WindowsManager::Terminate() {
    for (int i = 0; i < windows.size(); ++i)
    {
        windows[i]->CloseWindow();
    }
    windows.clear();
    return true;
}
b8 WindowsManager::Tick(f64) {
    for (int i = 0; i < windows.size(); ++i)
    {
        // windows[i].ShowWindow();
        // windows[i]->Update(data, width, height);
    }
    return true;
}

#ifdef RT_SYSTEM_APPLE
MacOSXWindow* InnerCreateWindowMacOSX(u32 width, u32 height,u32 pos_x, u32 pos_y, u8 channel) {
    MacOSXWindow* window = new MacOSXWindow();
    // u8* buffer = new u8[width * height * channel];
    // for (size_t i = 0; i < width * height * channel; ++i) {
    //     buffer[i] = 0;
    //     if ((i + 1) % 4 == 0) buffer[i] = 255;
    // }
    window->Create("test", width, height, pos_x, pos_y);
    // window.cocoaView->SetHeight(height);
    // window.cocoaView->SetWidth(width);
    // window.cocoaView->SetChannel(channel);
    // window.cocoaView->SetBuffer(buffer);
    window->ShowWindow();
    return window;
};
#endif

#ifdef RT_SYSTEM_WINDOWS
IWindow* InnerCreateWindowWindows(uint32_t width, uint32_t height, u32 pos_x, u32 pos_y, uint8_t channel) {
    WindowsWindow* window = new WindowsWindow();
    window->Create("test", width, height, pos_x, pos_y);
    window->ShowWindow();
    return static_cast<IWindow*>(window);
};
#endif

u32 WindowsManager::RTCreateWindow(u32 width, u32 height, u32 pos_x, u32 pos_y, uint8_t channel) {
    #ifdef RT_SYSTEM_APPLE
    PlatformWindow* instance = InnerCreateWindowMacOSX(width, height,pos_x, pos_y, channel);
    u32 index = windows.size();
    windows.push_back(instance);
    return index;
#endif
#ifdef RT_SYSTEM_WINDOWS
    PlatformWindow instance = InnerCreateWindowWindows(width, height, pos_x, pos_y, channel);
    u32 index = windows.size();
    windows.push_back(instance);
    return index;
#endif
}

void WindowsManager::PassViewData(uint8_t* _data, size_t _size, size_t _width, size_t _height)
{
    delete[] data;
    data = new uint8_t[_size];
    std::memcpy(data, _data, _size);
    width = _width;
    height = _height;
}

PlatformWindow* WindowsManager::GetWindow(u32 index) {
    if (index < windows.size()) {
        return windows[index];
    }
    RT_LOG_ERROR("Index out of bounds when getting window: ", index);
    return nullptr;
}
}  // namespace ReiToEngine
