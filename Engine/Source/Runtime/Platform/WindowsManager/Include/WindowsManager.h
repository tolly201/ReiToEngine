#ifndef PLATFORM_INCLUDE_WINDOWS_MANAGER_H
#define PLATFORM_INCLUDE_WINDOWS_MANAGER_H
#include "Platform/Singleton/RuntimeSingleton.h"
#include <cstddef>
#include <vector>
#include <cstdint>
#include "Platform/HAL/Window/Include/IWindow.h"
namespace ReiToEngine{

class WindowsManager : public Runtime_Singleton<WindowsManager>
{
public:
    WindowsManager();
    void Initialize() override;
    void Terminate() override;
    void Tick() override;
    uint32_t RTCreateWindow(uint32_t width = 100, uint32_t height = 100, u32 pos_x = 100, u32 pos_y = 100, uint8_t channel = 4);
    void PassViewData(uint8_t* _data, size_t _size, size_t _width, size_t _height);
private:
    std::vector<IWindow*> windows;

    uint8_t* data = nullptr;
    size_t size = 0;
    size_t width = 0;
    size_t height = 0;
};
}
#endif
