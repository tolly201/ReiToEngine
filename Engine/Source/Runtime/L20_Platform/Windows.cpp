#include "PlatformFunctions.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
#include "L20_Platform/L37_Window/Functions.h"
#include "L20_Platform/L37_Window/Windows/WindowsWindow.h"
#include "L20_Platform/L35_Input/Include.h"

b8 RT_Platform_Initialize(){
    RT_Platform_MainWindowInitialize();
    return true;

}
void RT_Platform_Terminate()
{
    RT_Platform_MainWindowTerminate();
}

b8 RT_Platform_PumpMessage()
{
    MSG message;
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return true;
}

#endif
