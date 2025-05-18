#include "../Include/TimerWindows.h"
#ifdef  RT_SYSTEM_WINDOWS
#include <windows.h>

static f64 clock_frequency = 0.0;
static LARGE_INTEGER frequency;
static LARGE_INTEGER start_time;

f64 RT_HAL_AbsoluteTime()
{
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_frequency;
}

void RT_HAL_Sleep(u64 ms)
{
    Sleep(ms);
}

void RT_HAL_InitTime()
{
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);
}

#endif
