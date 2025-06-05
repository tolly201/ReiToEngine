#include "Functions.h"
#ifdef RT_SYSTEM_LINUX
#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else
#include <unistd.h>
#endif
f64 RT_Platform_AbsoluteTime(){return 0.0;}
void RT_Platform_InitTime(){}
void RT_Platform_Sleep(u64 ms)
{
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, nullptr);
#else
    if (ms >= 1000)
    {
        sleep(ms/1000);
    }
    usleep((ms % 1000) * 1000);
#endif
}
#endif
