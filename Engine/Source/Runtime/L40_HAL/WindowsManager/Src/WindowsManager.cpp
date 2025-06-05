#include "../Include/WindowsManager.h"
#include <cstring>

#include "L20_Platform/L37_Window/IWindow.h"
#include "L20_Platform/L37_Window/IWindowView.h"

#ifdef RT_SYSTEM_APPLE
#include "L20_Platform/L37_Window/MACOSX/MacOSXView.h"
#include "L20_Platform/L37_Window/MACOSX/MacOSXWindow.h"
#endif

#ifdef RT_SYSTEM_WINDOWS
#include "L60_Core/HAL/Window/Include/Windows/WindowsWindow.h"
#endif

namespace ReiToEngine {
WindowsManager::WindowsManager() = default;
b8 WindowsManager::Initialize() {
    return true;
}
b8 WindowsManager::Terminate() {
    for (IWindow*& window_ptr:windows)
    {
        window_ptr->CloseWindow();
        delete window_ptr;
    }
    windows.clear();
    return true;
}
b8 WindowsManager::Tick(f64) {
    for (IWindow*& window_ptr:windows)
    {
        window_ptr->ShowWindow();
        window_ptr->Update(data, width, height);
    }
    return true;
}

#ifdef RT_SYSTEM_APPLE
IWindow* InnerCreateWindowMacOSX(uint32_t width, uint32_t height, uint8_t channel) {
    MacOSXWindow* window = new MacOSXWindow();
    uint8_t* buffer = new uint8[width * height * channel];
    for (size_t i = 0; i < width * height * channel; ++i) {
        buffer[i] = 0;
        if ((i + 1) % 4 == 0) buffer[i] = 255;
    }
    window->Create("test", width, height, 100,100);
    window->cocoaView->SetHeight(height);
    window->cocoaView->SetWidth(width);
    window->cocoaView->SetChannel(channel);
    window->cocoaView->SetBuffer(buffer);
    window->ShowWindow();
    return static_cast<IWindow*>(window);
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

uint32_t WindowsManager::RTCreateWindow(uint32_t width, uint32_t height, u32 pos_x, u32 pos_y, uint8_t channel) {
    IWindow* instance = nullptr;
    uint32_t index = windows.size();
#ifdef RT_SYSTEM_APPLE
    instance = InnerCreateWindowMacOSX(width, height, channel);
#endif
#ifdef RT_SYSTEM_WINDOWS
    instance = InnerCreateWindowWindows(width, height, pos_x, pos_y, channel);
#endif
    windows.push_back(instance);
    return index;
}

void WindowsManager::PassViewData(uint8_t* _data, size_t _size, size_t _width, size_t _height)
{
    delete[] data;
    data = new uint8_t[_size];
    std::memcpy(data, _data, _size);
    width = _width;
    height = _height;
}
}  // namespace ReiToEngine
