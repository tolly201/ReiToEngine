#ifndef CORE_APPLICATION_RT_WINDOWS_APPLICATION_H
#define CORE_APPLICATION_RT_WINDOWS_APPLICATION_H
#include "RTApplication.h"
#ifdef RT_SYSTEM_WINDOWS
    #include <windows.h>
namespace ReiToEngine{
class RTWindowsApplication : public RTApplication
{
    friend RTApplication;
    void Initialize(ApplicatonConfig&) override;
    void Run() override;
    void Tick() override;
    void Terminate() override;
protected:
    static RTWindowsApplication instance;
    HINSTANCE hInstance;
};

}
#endif
#endif
