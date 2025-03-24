#ifndef CORE_CORE_H
#define CORE_CORE_H
#include "Core/Platform/Memory/Memory.h"
#include "Core/Common/FileManager/CFileManager.h"
#include "Core/Common/ImageParser/STBImageParser.h"
#include "Core/HAL/System/System.h"
#include "Core/Application/Include/RTApplication.h"
#endif


void* operator new(size_t uiSize);

void* operator new[] (size_t uiSize);

void operator delete (void* pvAddr)noexcept;

void operator delete[] (void* pvAddr)noexcept;

// 重载 malloc 和 free
RT_FORCEINLINE void* malloc(size_t size);

RT_FORCEINLINE void free(void* ptr);
