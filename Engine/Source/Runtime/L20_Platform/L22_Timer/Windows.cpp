#include "Functions.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>

f64 RT_Platform_AbsoluteTime(){return 0.0f;}
void RT_Platform_InitTime(){}
void RT_Platform_Sleep(u64 ms)
{
    Sleep(ms);
}


#endif
