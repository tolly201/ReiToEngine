#ifndef CORE_APPLICATION_RT_WINDOWS_APPLICATION_H
#define CORE_APPLICATION_RT_WINDOWS_APPLICATION_H
#include "RTApplication.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
namespace ReiToEngine{
class RTENGINE_API RTWindowsApplication : public RTApplication
{
    friend RTApplication;
    b8 Initialize(IGame*) override;
    b8 StartGame() override;
    b8 Run() override;
    void Terminate() override;
protected:
    static RTWindowsApplication instance;
    RTWindowsApplication() = default;
    ~RTWindowsApplication() = default;
    HINSTANCE hInstance;

};

}
#endif
#endif
