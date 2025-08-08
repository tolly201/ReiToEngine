#ifndef PLATFORM_INCLUDE_WINDOWS_MANAGER_H
#define PLATFORM_INCLUDE_WINDOWS_MANAGER_H
#include "L20_Platform/L37_Window/IWindow.h"
#include "L20_Platform/L31_SingletonFactory/RuntimeSingleton.h"
#include <L20_Platform/L32_Containers/Vector.h>
namespace ReiToEngine{

class WindowsManager : public Runtime_Singleton<WindowsManager>
{
public:
    WindowsManager();
    b8 Initialize() override;
    b8 Terminate() override;
    b8 Tick(f64) override;
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
