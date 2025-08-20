#include "Functions.h"
#ifdef RT_SYSTEM_APPLE
#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif
f64 RT_Platform_AbsoluteTime()
{
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
#else
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec + tv.tv_usec / 1e6;
#endif
}

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
