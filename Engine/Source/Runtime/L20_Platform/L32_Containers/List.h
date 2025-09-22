#ifndef PLATFORM_CONTAINERS_LIST_H
#define PLATFORM_CONTAINERS_LIST_H
#include "L0_Macro/Include.h"
#include "L20_Platform/L31_SingletonFactory/SingletonFactory.h"
#include "L20_Platform/L30_MemoryManager/Memory.h"
#include <new>
#include <type_traits>

namespace ReiToEngine
{

// 一个轻量自管内存的线性容器，接口贴近 std::vector（不依赖 STL 容器）
template <typename T>
class RTENGINE_API List
{
public:
    using value_type = T;

    List() = default;
    ~List() { destroy_and_free(); }

    // 拷贝/移动
    List(const List& other) { copy_from(other); }
    List& operator=(const List& other) {
        if (this != &other) { destroy_and_free(); copy_from(other); }
        return *this;
    }
    List(List&& other) noexcept { move_from(other); }
    List& operator=(List&& other) noexcept {
        if (this != &other) { destroy_and_free(); move_from(other); }
        return *this;
    }

    // 容量/大小
    [[nodiscard]] u64 size() const noexcept { return _size; }
    [[nodiscard]] u64 capacity() const noexcept { return _capacity; }
    [[nodiscard]] b8  empty() const noexcept { return _size == 0; }

    void clear() noexcept { destroy_range(0, _size); _size = 0; }
    void reserve(u64 new_cap) { if (new_cap > _capacity) reallocate(new_cap); }

    // 注意：当 T 不可默认构造时，调用该重载将导致编译失败，语义与 std::vector 一致
    void resize(u64 new_size) {
        if (new_size < _size) { destroy_range(new_size, _size); _size = new_size; return; }
        if (new_size > _capacity) reallocate(grow_to_fit(new_size));
        // 对新增元素进行值初始化
        for (u64 i = _size; i < new_size; ++i) ::new (static_cast<void*>(_data + i)) T();
        _size = new_size;
    }
    void resize(u64 new_size, const T& value) {
        if (new_size < _size) { destroy_range(new_size, _size); _size = new_size; return; }
        if (new_size > _capacity) reallocate(grow_to_fit(new_size));
        for (u64 i = _size; i < new_size; ++i) ::new (static_cast<void*>(_data + i)) T(value);
        _size = new_size;
    }

    // 插入
    void push_back(const T& v) { emplace_back(v); }
    void push_back(T&& v)      { emplace_back(static_cast<T&&>(v)); }

    template <typename... Args>
    T& emplace_back(Args&&... args) {
        if (_size == _capacity) reallocate(grow_auto());
        T* slot = _data + _size;
        ::new (static_cast<void*>(slot)) T(static_cast<Args&&>(args)...);
        ++_size;
        return *slot;
    }

    void pop_back() {
        if (_size == 0) return;
        (_data + (_size - 1))->~T();
        --_size;
    }

    // 访问
    T& operator[](u64 i) { return _data[i]; }
    const T& operator[](u64 i) const { return _data[i]; }

    T& front() { return _data[0]; }
    const T& front() const { return _data[0]; }
    T& back() { return _data[_size - 1]; }
    const T& back() const { return _data[_size - 1]; }

    T* data() noexcept { return _data; }
    const T* data() const noexcept { return _data; }

    // 迭代（返回裸指针，适配基于范围的 for）
    T* begin() noexcept { return _data; }
    T* end()   noexcept { return _data + _size; }
    const T* begin() const noexcept { return _data; }
    const T* end()   const noexcept { return _data + _size; }

    // 无序删除：O(1)，将末尾元素搬到被删位置
    void erase_unstable(u64 index) {
        if (index >= _size) return;
        _data[index].~T();
        if (index != _size - 1) {
            ::new (static_cast<void*>(_data + index)) T(static_cast<T&&>(_data[_size - 1]));
            _data[_size - 1].~T();
        }
        --_size;
    }

private:
    // 内部工具
    static constexpr u8 alignment() { return static_cast<u8>(alignof(T)); }

    void destroy_and_free() {
        destroy_range(0, _size);
        if (_data) {
            GetMemoryManager().Free(_data, _capacity * sizeof(T), RT_MEMORY_TAG::CONTAINER);
            _data = nullptr; _capacity = 0; _size = 0;
        }
    }

    void copy_from(const List& other) {
        if (other._size == 0) { _data = nullptr; _size = _capacity = 0; return; }
        _data = static_cast<T*>(GetMemoryManager().Allocate(other._size * sizeof(T), alignment(), RT_MEMORY_TAG::CONTAINER));
        _capacity = _size = other._size;
        if constexpr (std::is_trivially_copyable_v<T>) {
            RT_Platform_SYSCopyMemory(_data, other._data, _size * sizeof(T));
        } else {
            for (u64 i = 0; i < _size; ++i) ::new (static_cast<void*>(_data + i)) T(other._data[i]);
        }
    }

    void move_from(List& other) noexcept {
        _data = other._data; _size = other._size; _capacity = other._capacity;
        other._data = nullptr; other._size = other._capacity = 0;
    }

    void destroy_range(u64 b, u64 e) {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for (u64 i = b; i < e; ++i) (_data + i)->~T();
        }
    }

    u64 grow_auto() const {
        if (_capacity == 0) return 4;
        // 小容量翻倍，大容量 1.5x
        return _capacity < 1024 ? (_capacity * 2) : (_capacity + _capacity / 2);
    }
    u64 grow_to_fit(u64 min_needed) const {
        u64 cap = _capacity ? _capacity : 4;
        while (cap < min_needed) cap = (cap < 1024 ? cap * 2 : cap + cap / 2);
        return cap;
    }

    void reallocate(u64 new_cap) {
        T* new_mem = static_cast<T*>(GetMemoryManager().Allocate(new_cap * sizeof(T), alignment(), RT_MEMORY_TAG::CONTAINER));
        // 搬迁
        if constexpr (std::is_trivially_copyable_v<T>) {
            if (_size) RT_Platform_SYSCopyMemory(new_mem, _data, _size * sizeof(T));
        } else {
            for (u64 i = 0; i < _size; ++i) {
                ::new (static_cast<void*>(new_mem + i)) T(static_cast<T&&>(_data[i]));
                (_data + i)->~T();
            }
        }
        // 释放旧内存
        if (_data) GetMemoryManager().Free(_data, _capacity * sizeof(T), RT_MEMORY_TAG::CONTAINER);
        _data = new_mem; _capacity = new_cap;
    }

private:
    T*  _data = nullptr;
    u64 _size = 0;
    u64 _capacity = 0;
};

} // namespace ReiToEngine

#endif
