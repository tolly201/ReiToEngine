#ifndef PLATFORM_OBJECTS_STRING_FUNCTION_H
#define PLATFORM_OBJECTS_STRING_FUNCTION_H
#include "L0_Macro/Include.h"
namespace ReiToEngine {
RTENGINE_API u64 RT_CharsLength(const char* str);
RTENGINE_API b8 RT_CharsCompare(const char* str1, const char*
str2);
RTENGINE_API char* RT_CharsDumpicate(const char* str);
}

#endif
