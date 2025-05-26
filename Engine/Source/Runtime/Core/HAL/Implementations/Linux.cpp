#include "../PlatformDefines.h"
#ifdef RT_SYSTEM_LINUX
#include "../Window/Include/Linux/Wayland/WaylandWindow.h"

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>
#else
#include <unistd.h>
#include <iostream>
#include <sys/mman.h> // For mmap, munmap
#include <unistd.h>  // For sysconf, _SC_PAGE_SIZE
#endif

b8 RT_HAL_Initialize(RT_HAL_MAIN_WINDOW& window, const char* title, u32 width, u32 height, u32 pos_x, u32 pos_y)
{
    window.main_window = new WayLandWindow();
    window.main_window->Create(title, width, height, pos_x, pos_y);
    window.main_window->ShowWindow();
    return true;
}

void RT_HAL_Terminate(RT_HAL_MAIN_WINDOW& window)
{
    if (window.main_window != nullptr)
    {
        window.main_window->CloseWindow();
        delete window.main_window;
        window.main_window = nullptr;
    }
}

void RT_HAL_PumpMessage(RT_HAL_MAIN_WINDOW& window);

void* RT_HAL_SYSAlloc(u64 uiSize, u8 alignment, b8 aligned, b8 isArray)
{
    {
        size_t pageSize = sysconf(_SC_PAGE_SIZE);
        size_t allocateSize = (uiSize + pageSize - 1) / pageSize * pageSize;
        void* ptr = mmap(nullptr, allocateSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (ptr == MAP_FAILED)
        {
            return nullptr;
        }
        printf("sysallo\n");
        return ptr;
    }
}

void RT_HAL_SYSFree(void* pMem, u64 uiSize)
{
    if (pMem != nullptr)
    {
        size_t pageSize = sysconf(_SC_PAGE_SIZE); // 获取系统页大小
        size_t freeSize = (uiSize + pageSize - 1) / pageSize * pageSize; // 页对齐释放大小 (需要和分配时大小一致)
        printf("sysfree\n");
        std::cout <<"指针值:"<< pMem << std::endl;
        munmap(pMem, freeSize);
    }
}

void RT_HAL_SYSZeroMemory(void* pMem, u64 uiSize)
{
    memset(pMem, 0, uiSize);
}

void RT_HAL_SYSCopyMemory(void* pDest, const void* pSrc, u64 uiSize)
{
    memcpy(pDest, pSrc, uiSize);
}

void RT_HAL_SYSMoveMemory(void* pDest, const void* pSrc, u64 uiSize)
{
    memmove(pDest, pSrc, uiSize);
}

void RT_HAL_SYSSetMemory(void* pMem, u8 value, u64 uiSize)
{
    memset(pMem, value, uiSize);
}

void RT_HAL_ConsoleWrite(const char* message, u8 color)
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

void RT_HAL_ConsoleWriteError(const char* message, u8 color)
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

f64 RT_HAL_AbsoluteTime();
void RT_HAL_InitTime();
void RT_HAL_Sleep(u64 ms)
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
