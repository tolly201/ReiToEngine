#include "PlatformFunctions.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
#include "L20_Platform/L37_Window/Functions.h"
#include "L20_Platform/L37_Window/Windows/WindowsWindow.h"
#include "L20_Platform/L35_Input/Include.h"
#include "windows.h"
b8 RT_Platform_Initialize(RT_Platform_State*& platform_state){
    platform_state = new RT_Platform_State();
    platform_state->hInstance = GetModuleHandleW(nullptr);
    RT_Platform_MainWindowInitialize(platform_state);
    return true;

}
void RT_Platform_Terminate(RT_Platform_State*& platform_state)
{
    delete platform_state;
    RT_Platform_MainWindowTerminate(platform_state);
}

b8 RT_Platform_PumpMessage(RT_Platform_State*& platform_state)
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
