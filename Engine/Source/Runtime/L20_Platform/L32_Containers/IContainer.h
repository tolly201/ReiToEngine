#ifndef PLATFORM_CONTAINERS_ICONTAINER_H
#define PLATFORM_CONTAINERS_ICONTAINER_H
#include "L0_Macro/Include.h"
#include "L20_Platform/L31_SingletonFactory/SingletonFactory.h"
#include "L20_Platform/L30_MemoryManager/Memory.h"

namespace ReiToEngine {

// 轻量级占位基类：用于与现有容器保持兼容。
// 当前不提供任何行为，仅作为继承锚点，后续可逐步移除继承关系。
template <typename Derived>
class IContainer {
public:
    IContainer() = default;
    ~IContainer() = default;
    IContainer(const IContainer&) = default;
    IContainer& operator=(const IContainer&) = default;
    IContainer(IContainer&&) noexcept = default;
    IContainer& operator=(IContainer&&) noexcept = default;
};

template <typename T>
inline T* rt_alloc_array(u64 count, RT_MEMORY_TAG tag = RT_MEMORY_TAG::CONTAINER) {
    if (count == 0) return nullptr;
    void* p = GetMemoryManager().Allocate(count * sizeof(T), static_cast<u8>(alignof(T)), tag);
    return static_cast<T*>(p);
}

template <typename T>
inline void rt_free_array(T* ptr, u64 allocated_count, RT_MEMORY_TAG tag = RT_MEMORY_TAG::CONTAINER) {
    if (!ptr) return;
    GetMemoryManager().Free(ptr, allocated_count * sizeof(T), tag);
}
} // namespace ReiToEngine
#endif
