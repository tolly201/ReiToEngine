#include "Functions.h"
#ifdef RT_SYSTEM_LINUX
#include "./Include/Linux/Wayland/WaylandWindow.h"


b8 RT_Platform_MainWindowInitialize()
{

    return true;
}

void RT_Platform_MainWindowTerminate()
{
}

b8 RT_Platform_MainWindowPumpMessage()
{}

#endif
