#include "Functions.h"
#ifdef RT_SYSTEM_LINUX
#include <cstdio>
void RT_Platform_ConsoleWrite(const char* message, u8 color)
{
    static const char* color_levels[6] = {
    "0;41", // FATAL 红色加粗
    "1;31", // ERROR 红色
    "1;33", // WARN  黄色加粗
    "1;32", // INFO  绿色
    "1;34", // DEBUG 青色
    "1;30"     // TRACE 默认
    };
    fprintf(stdout, "\033[%sm%s\033[0m", color_levels[color], message);
    fflush(stdout);
}

void RT_Platform_ConsoleWriteError(const char* message, u8 color)
{
    static const char* color_levels[6] = {
    "0;41", // FATAL 红色加粗
    "1;31", // ERROR 红色
    "1;33", // WARN  黄色加粗
    "1;32", // INFO  绿色
    "1;34", // DEBUG 青色
    "1;30"     // TRACE 默认
    };
    fprintf(stderr, "\033[%sm%s\033[0m", color_levels[color], message);
    fflush(stderr);
}
#endif
