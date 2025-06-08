#include "Functions.h"
#ifdef  RT_SYSTEM_WINDOWS
#include <windows.h>

static f64 clock_frequency = 0.0;
static LARGE_INTEGER frequency;
static LARGE_INTEGER start_time;

f64 RT_Platform_AbsoluteTime()
{
    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_frequency;
}
void RT_Platform_InitTime()
{
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);
}
#endif
