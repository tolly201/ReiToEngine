#ifndef PLATFORM_OBJECTS_STRING_H
#define PLATFORM_OBJECTS_STRING_H
#include "L0_Macro/Include.h"
namespace ReiToEngine {
RTENGINE_API u64 RT_StrLen(const char* str);
RTENGINE_API b8 RT_StrCmp(const char* str1, const char*
str2);

RTENGINE_API char* RT_StrDup(const char* str);
}

#endif
