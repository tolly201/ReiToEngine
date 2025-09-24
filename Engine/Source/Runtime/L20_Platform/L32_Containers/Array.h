#ifndef PLATFORM_CONTAINERS_ARRAY_H
#define PLATFORM_CONTAINERS_ARRAY_H

#include "L0_Macro/Include.h"
#include <cstddef>
#include <type_traits>

namespace ReiToEngine {

template <typename T, u64 N>
class Array {
public:
    using value_type = T; using size_type = u64; using reference = T&; using const_reference = const T&;

    static constexpr size_type size_static() noexcept { return N; }
    [[nodiscard]] static constexpr size_type max_size() noexcept { return N; }

    // 容量/状态
    [[nodiscard]] constexpr size_type size() const noexcept { return N; }
    [[nodiscard]] constexpr b8 empty() const noexcept { return N == 0; }

    // 迭代
    T* begin() noexcept { return _data; }
    T* end() noexcept { return _data + N; }
    const T* begin() const noexcept { return _data; }
    const T* end() const noexcept { return _data + N; }
    const T* cbegin() const noexcept { return _data; }
    const T* cend() const noexcept { return _data + N; }

    // 访问
    reference operator[](size_type i) { return _data[i]; }
    const_reference operator[](size_type i) const { return _data[i]; }
    reference front() { return _data[0]; }
    const_reference front() const { return _data[0]; }
    reference back() { return _data[N-1]; }
    const_reference back() const { return _data[N-1]; }
    T* data() noexcept { return _data; }
    const T* data() const noexcept { return _data; }

    // 填充/交换
    void fill(const T& v){ for(size_type i=0;i<N;++i) _data[i] = v; }
    void swap(Array& o){ for(size_type i=0;i<N;++i){ T tmp = static_cast<T&&>(_data[i]); _data[i] = static_cast<T&&>(o._data[i]); o._data[i] = static_cast<T&&>(tmp); } }

private:
    T _data[N ? N : 1]{}; // N==0 时占位
};

} // namespace ReiToEngine

#endif
