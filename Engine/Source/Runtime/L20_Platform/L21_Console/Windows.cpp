#include "Functions.h"
#ifdef RT_SYSTEM_WINDOWS
#include <windows.h>
void RT_Platform_ConsoleWrite(const char* message, u8 color)
{
    static u8 color_levels[6] = {0x0c | 0x40, 0x0c, 0x0E, 0x0A, 0x0B, 0x07};
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console_handle, &csbi);
    WORD original_attributes = csbi.wAttributes;

    SetConsoleTextAttribute(console_handle, color_levels[color]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, nullptr);
    SetConsoleTextAttribute(console_handle, original_attributes);
}

void RT_Platform_ConsoleWriteError(const char* message, u8 color)
{
    static u8 color_levels[6] = {0x0c | 0x40, 0x0c, 0x0E, 0x0A, 0x0B, 0x07};
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console_handle, &csbi);
    WORD original_attributes = csbi.wAttributes;

    SetConsoleTextAttribute(console_handle, color_levels[color]);

    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, nullptr);

    SetConsoleTextAttribute(console_handle, original_attributes);
}
#endif
