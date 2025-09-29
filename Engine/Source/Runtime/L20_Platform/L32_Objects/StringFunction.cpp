#include "String.h"
#include "L20_Platform/L31_SingletonFactory/SingletonFactory.h"
#include <string.h>
namespace ReiToEngine {

    u64 RT_CharsLength(const char* str)
    {
        return strlen(str);
    }
    b8 RT_CharsCompare(const char* str1, const char*
        str2)
        {
            return strcmp(str1, str2) == 0;
        }

        char* RT_CharsDumpicate(const char* str)
        {
            if (!str) return nullptr;
            u64 len = RT_CharsLength(str);
            char* new_str = (char*)GetMemoryManager().Allocate(len + 1, alignof(char), RT_MEMORY_TAG::STRING);
            if (!new_str) return nullptr;
            memcpy(new_str, str, len + 1);
            return new_str;
        }
    }
