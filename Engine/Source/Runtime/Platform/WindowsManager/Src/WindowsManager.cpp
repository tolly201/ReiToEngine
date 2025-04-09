#include "../Include/WindowsManager.h"
#include <Security/Security.h>
#include <_types/_uint32_t.h>

#include "Platform/HAL/Window/Include/IWindow.h"
#include "Platform/HAL/Window/Include/IWindowView.h"

#ifdef RT_SYSTEM_APPLE
#include "Platform/HAL/Window/Include/MACOSX/MacOSXView.h"
#include "Platform/HAL/Window/Include/MACOSX/MacOSXWindow.h"
#endif
namespace ReiToEngine {
WindowsManager::WindowsManager() = default;
void WindowsManager::Initialize() {}
void WindowsManager::Terminate() {
    for (IWindow*& window_ptr:windows)
    {
        window_ptr->CloseWindow();
        delete window_ptr;
    }
    windows.clear();
}
void WindowsManager::Tick() {
    // for (IWindow*& window_ptr:windows)
    // {
    //     window_ptr->Update(nullptr, 0, 0);
    // }
}

#ifdef RT_SYSTEM_APPLE
IWindow* InnerCreateWindowMacOSX(uint32_t width, uint32_t height, uint8_t channel) {
    MacOSXWindow* window = new MacOSXWindow();
    uint8_t* buffer = new uint8[width * height * channel];
    for (size_t i = 0; i < width * height * channel; ++i) {
        buffer[i] = 0;
        if ((i + 1) % 4 == 0) buffer[i] = 255;
    }
    window->Create("test", width, height);
    window->cocoaView->SetHeight(height);
    window->cocoaView->SetWidth(width);
    window->cocoaView->SetChannel(channel);
    window->cocoaView->SetBuffer(buffer);
    window->ShowWindow();
    return static_cast<IWindow*>(window);
};
#endif

uint32_t WindowsManager::CreateWindow(uint32_t width, uint32_t height, uint8_t channel) {
    IWindow* instance = nullptr;
    uint32_t index = windows.size();
#ifdef RT_SYSTEM_APPLE
    instance = InnerCreateWindowMacOSX(width, height, channel);
#endif
    windows.push_back(instance);
    return index;
}

void WindowsManager::AddKeyDownCallback(uint32_t index, KeyDownCallback callback)
{
    IWindow* ptr= windows[index];
    ptr->AddKeyDownCallback(callback);
}

}  // namespace ReiToEngine
