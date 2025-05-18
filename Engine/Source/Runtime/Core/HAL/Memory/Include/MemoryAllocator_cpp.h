#ifndef CORE_HAL_MEMORY_MEMORYALLOCATOR_H
#define CORE_HAL_MEMORY_MEMORYALLOCATOR_H
#include "Core/Macro/Macro.h"
#include <cstddef>
#include <iostream>
#ifdef RT_SYSTEM_WINDOWS
#else
#include <sys/mman.h> // For mmap, munmap
#include <unistd.h>  // For sysconf, _SC_PAGE_SIZE
void* RTSysAlloc(size_t uiSize)
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

    void RTSysFree(void* pMem, size_t uiSize)
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
#endif
#endif
