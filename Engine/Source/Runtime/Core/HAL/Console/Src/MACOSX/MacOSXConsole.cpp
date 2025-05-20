#include "../../Include/MACOSX/MacOSXWindow.h"

#ifdef RT_SYSTEM_APPLE
#include <cstdio>
// 6级日志颜色，和windows实现保持一致
static const char* color_levels[6] = {
    "\033[1;37;41m", // FATAL 红色加粗
    "\033[0;31m", // ERROR 红色
    "\033[0;33m", // WARN  黄色加粗
    "\033[0;32m", // INFO  绿色
    "\033[0;36m", // DEBUG 青色
    "\033[0;90m"     // TRACE 默认
};

void RT_HAL_ConsoleWrite(const char* message, u8 color)
{
    fprintf(stdout, "%s%s\033[0m", color_levels[color], message);
    fflush(stdout);
}
void RT_HAL_ConsoleWriteError(const char* message, u8 color)
{
    fprintf(stderr, "%s%s\033[0m", color_levels[color], message);
    fflush(stderr);
}
#endif
