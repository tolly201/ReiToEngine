#include "../Include/RTWindowsApplication.h"

#ifdef RT_SYSTEM_WINDOWS
using namespace ReiToEngine;



void RTWindowsApplication::Initialize(ApplicatonConfig& app_config)
{
    RTApplication::Initialize(app_config);
    hInstance = GetModuleHandleW(0);
}

void RTWindowsApplication::Run()
{
    RTApplication::Run();
}
void RTWindowsApplication::Tick()
{
    RTApplication::Tick();

}
void RTWindowsApplication::Terminate()
{

    RTApplication::Terminate();
    // Unregister the window class
}
#endif
