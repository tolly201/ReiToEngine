#ifndef CORE_MATH_SVector2_H
#define CORE_MATH_SVector2_H
#include <cstdint>
#include <initializer_list>
#include <cmath>
#include "L20_Platform/L23_Logger/Include.h"
#include <new>
#include "L20_Platform/L31_SingletonFactory/SingletonFactory.h"
namespace ReiToEngine
{
template <typename T>
class SVector2 final
{
public:
    static constexpr uint8_t Dimension = 2;
    // Custom memory management
    static void* operator new(std::size_t sz)
    {
        return GetMemoryManager().Allocate(sz, static_cast<u8>(alignof(SVector2)), RT_MEMORY_TAG::MATH);
    }
    static void operator delete(void* p, std::size_t sz) noexcept
    {
        if (p) GetMemoryManager().Free(p, sz, RT_MEMORY_TAG::MATH);
    }
#if __cpp_aligned_new
    static void* operator new(std::size_t sz, std::align_val_t al)
    {
        std::size_t a = static_cast<std::size_t>(al);
        u8 align_u8 = static_cast<u8>(a > 255 ? 255 : a);
        return GetMemoryManager().Allocate(sz, align_u8, RT_MEMORY_TAG::MATH);
    }
    static void operator delete(void* p, std::size_t sz, std::align_val_t) noexcept
    {
        if (p) GetMemoryManager().Free(p, sz, RT_MEMORY_TAG::MATH);
    }
#endif
    static void* operator new[](std::size_t sz)
    {
        return GetMemoryManager().Allocate(sz, static_cast<u8>(alignof(SVector2)), RT_MEMORY_TAG::MATH);
    }
    static void operator delete[](void* p, std::size_t sz) noexcept
    {
        if (p) GetMemoryManager().Free(p, sz, RT_MEMORY_TAG::MATH);
    }
    SVector2(const std::initializer_list<T>& init) noexcept
    {
        x = y = T{0};
        int list_size = init.size();
        list_size = list_size > 2? 2 : list_size;
        if (list_size > 0) x = init.begin()[0];
        if (list_size > 1) y = init.begin()[1];
    }
    SVector2() noexcept
    {
        x = y = T{0};
    }
    SVector2(T x, T y) noexcept:
        x(x), y(y) {};
    SVector2(T val) noexcept:x(val), y(val) {};
    SVector2(const SVector2<T>& other) = default;
    SVector2(SVector2<T>&&)noexcept = default;
    // SVector2<T>ement functions from IVector (CRTP style, returning SVector<T, DIM>& and SVector<T, DIM>)
    SVector2<T>& operator+=(const SVector2<T>& other) noexcept
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    SVector2<T>& operator-=(const SVector2<T>& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    SVector2<T>& operator*=(T scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    SVector2<T>& operator/=(T scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    [[nodiscard]] SVector2<T> operator+(const SVector2<T>& other) const noexcept
    {
        return SVector2<T>{x + other.x, y + other.y};
    }
    [[nodiscard]] SVector2<T> operator-(const SVector2<T>& other) const noexcept
    {
        return SVector2<T>{x - other.x, y - other.y};
    }
    [[nodiscard]] SVector2<T> operator*(T scalar) const noexcept
    {
        return SVector2<T>{x * scalar, y * scalar};
    }
    [[nodiscard]] SVector2<T> operator/(T scalar) const noexcept
    {
        return SVector2<T>{x / scalar, y / scalar};
    }
    [[nodiscard]] T operator*(const SVector2<T>& other) const noexcept {
        return x*other.x + y*other.y;
    }

    T& operator[](int index) noexcept {
        RT_ASSERT(index >= 0 && index < 2);
        return index == 0 ? x : y;
    }

    [[nodiscard]] const T& operator[](int index) const noexcept {
        RT_ASSERT(index >= 0 && index < 2);
        return index == 0 ? x : y;
    }

    [[nodiscard]] bool operator==(const SVector2<T>& other) const noexcept
    {
        const T eps = static_cast<T>(RT_COMPARE_PRECISION);
        return std::abs(x - other.x) < eps && std::abs(y - other.y) < eps;
    }
    [[nodiscard]] bool operator!=(const SVector2<T>& other) const noexcept
    {
        return !(*this==other);
    }
    friend std::ostream& operator<<(std::ostream& os, const SVector2<T>& vec)
    {
        os << "(" << vec[0] << ", " << vec[1] << ")";
        return os;
    }

    SVector2<T>& operator=(const SVector2<T>& other) noexcept
    {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }

    [[nodiscard]] T dot(const SVector2<T>& other) const noexcept
    {
        return *this*other;
    }

    [[nodiscard]] T cross2D(const SVector2<T>& other) const noexcept
    {
        return x*other.y - y*other.x;
    }

    [[nodiscard]] SVector2<T> cross4D(const SVector2<T>& other) const noexcept {
        return SVector2<T>();
    }

    [[nodiscard]] SVector2<T> cross3D(const SVector2<T>& other) const noexcept {
        return SVector2<T>(x*other.y - y*other.x, T{0});
    }

    [[nodiscard]] SVector2<T> normalize() const noexcept
    {
        const T len = std::sqrt(x*x + y*y);
        if (len == static_cast<T>(0)) {
            return SVector2<T>();
        }
        SVector2<T> ret = {x / len, y / len};
        return ret;
    }

    void normalizeSelf() noexcept
    {
        const T len = std::sqrt(x*x + y*y);
        if (len == static_cast<T>(0)) {
            x = y = T{0};
            return;
        }
        x = x / len;
        y = y / len;
    }
    [[nodiscard]] T length() const noexcept
    {
        return std::sqrt(x*x + y*y);
    }
    [[nodiscard]] T lengthSquared() const noexcept
    {
        return x*x + y*y;
    }
    [[nodiscard]] bool isZero() const noexcept
    {
        const T eps = static_cast<T>(RT_COMPARE_PRECISION);
        return this->lengthSquared() < eps;
    }
    [[nodiscard]] bool isNormalized() const noexcept
    {
        const T eps = static_cast<T>(RT_COMPARE_PRECISION);
        return std::abs(this->lengthSquared() - static_cast<T>(1)) < eps;
    }
    [[nodiscard]] T distance(const SVector2<T>& other) const noexcept
    {
        return std::sqrt(distanceSquared(other));
    }
    [[nodiscard]] T distanceSquared(const SVector2<T>& other) const noexcept
    {
        T dx = x - other.x;
        T dy = y - other.y;
        return dx * dx + dy * dy;
    }
    [[nodiscard]] SVector2<T> lerp(const SVector2<T>& other, T t) const noexcept
    {
        return {
            x + (other.x - x) * t,
            y + (other.y - y) * t,
            };
    }
    [[nodiscard]] SVector2<T> reflect(const SVector2<T>& normal) const noexcept
    {
        T dotProduct = this->dot(normal);
        return SVector2<T>{
            x - 2 * dotProduct * normal.x,
            y - 2 * dotProduct * normal.y,
        };
    }
    [[nodiscard]] SVector2<T> project(const SVector2<T>& normal) const noexcept
    {
        T dotProduct = this->dot(normal);
        T normalLengthSquared = normal.lengthSquared();
        return {
            (dotProduct / normalLengthSquared) * normal.x,
            (dotProduct / normalLengthSquared) * normal.y,
        };
    }
public:
    T x;
    T y;
};
}

#endif
