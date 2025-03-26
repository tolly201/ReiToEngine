#ifndef CORE_PLATFORM_INCLUDE_WINDOWS_MANAGER_H
#define CORE_PLATFORM_INCLUDE_WINDOWS_MANAGER_H
#include "Platform/Singleton/RuntimeSingleton.h"

namespace ReiToEngine{

class WindowsManager : public Runtime_Singleton<WindowsManager>
{
public:
    WindowsManager() = default;
    void Initialize() override {};
    void Terminate() override {};
    void Tick() override {};
    void CreateWindow();
};
}

#endif
