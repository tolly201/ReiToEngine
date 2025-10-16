#ifndef CORE_MATH_SVector3_H
#define CORE_MATH_SVector3_H
#include <cstdint>
#include <initializer_list>
#include <cmath>
#include <type_traits>
#include "L20_Platform/L21_Logger/Include.h"
#include <new>
#include "L20_Platform/L23_SingletonFactory/SingletonFactory.h"

namespace ReiToEngine
{
template <typename T>
class SVector3 final
{
public:
    static constexpr uint8_t Dimension = 3;
    // Custom memory management
    static void* operator new(std::size_t sz)
    {
        return GetMemoryManager().Allocate(sz, static_cast<u8>(alignof(SVector3)), RT_MEMORY_TAG::MATH);
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
        return GetMemoryManager().Allocate(sz, static_cast<u8>(alignof(SVector3)), RT_MEMORY_TAG::MATH);
    }
    static void operator delete[](void* p, std::size_t sz) noexcept
    {
        if (p) GetMemoryManager().Free(p, sz, RT_MEMORY_TAG::MATH);
    }
    // Note: For SIMD-friendly layout on float3, prefer packing into float4 or arrays-of-struct-of-arrays in higher-level containers.
    // We intentionally avoid adding implicit padding here to not change the ABI without a full audit.
    SVector3(const std::initializer_list<T>& init) noexcept
        : x(T{0}), y(T{0}), z(T{0})
        , r(x), g(y), b(z)
        , s(x), t(y), p(z)
        , u(x), v(y), w(z)
    {
        int list_size = static_cast<int>(init.size());
        list_size = list_size > 3 ? 3 : list_size;
        if (list_size > 0) x = init.begin()[0];
        if (list_size > 1) y = init.begin()[1];
        if (list_size > 2) z = init.begin()[2];
    }
    SVector3() noexcept
        : x(T{0}), y(T{0}), z(T{0})
        , r(x), g(y), b(z)
        , s(x), t(y), p(z)
        , u(x), v(y), w(z)
    {}
    SVector3(T x, T y, T z) noexcept
        : x(x), y(y), z(z)
        , r(this->x), g(this->y), b(this->z)
        , s(this->x), t(this->y), p(this->z)
        , u(this->x), v(this->y), w(this->z)
    {}
    SVector3(T val) noexcept
        : x(val), y(val), z(val)
        , r(this->x), g(this->y), b(this->z)
        , s(this->x), t(this->y), p(this->z)
        , u(this->x), v(this->y), w(this->z)
    {}
    SVector3(const SVector3<T>& other)
        : x(other.x), y(other.y), z(other.z)
        , r(this->x), g(this->y), b(this->z)
        , s(this->x), t(this->y), p(this->z)
        , u(this->x), v(this->y), w(this->z)
    {}
    SVector3(SVector3<T>&& other) noexcept
        : x(other.x), y(other.y), z(other.z)
        , r(this->x), g(this->y), b(this->z)
        , s(this->x), t(this->y), p(this->z)
        , u(this->x), v(this->y), w(this->z)
    {}
    // SVector3<T>ement functions from IVector (CRTP style, returning SVector<T, DIM>& and SVector<T, DIM>)
    SVector3<T>& operator+=(const SVector3<T>& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    SVector3<T>& operator-=(const SVector3<T>& other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    SVector3<T>& operator*=(T scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    SVector3<T>& operator/=(T scalar) noexcept
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    [[nodiscard]] SVector3<T> operator+(const SVector3<T>& other) const noexcept
    {
        return SVector3<T>{x + other.x, y + other.y, z + other.z};
    }
    [[nodiscard]] SVector3<T> operator-(const SVector3<T>& other) const noexcept
    {
        return SVector3<T>{x - other.x, y - other.y, z - other.z};
    }
    [[nodiscard]] SVector3<T> operator*(T scalar) const noexcept
    {
        return SVector3<T>{x * scalar, y * scalar, z * scalar};
    }
    [[nodiscard]] SVector3<T> operator/(T scalar) const noexcept
    {
        return SVector3<T>{x / scalar, y / scalar, z / scalar};
    }
    [[nodiscard]] T operator*(const SVector3<T>& other) const noexcept {
        return x*other.x + y*other.y + z*other.z;
    }

    T& operator[](int index) noexcept {
        RT_ASSERT_PLATFORM(index >= 0 && index < 3);
        return index == 0 ? x : (index == 1 ? y : z);
    }

    [[nodiscard]] const T& operator[](int index) const noexcept {
        RT_ASSERT_PLATFORM(index >= 0 && index < 3);
        return index == 0 ? x : (index == 1 ? y : z);
    }

    [[nodiscard]] bool operator==(const SVector3<T>& other) const noexcept
    {
        const T eps = static_cast<T>(RT_COMPARE_PRECISION);
        return std::abs(x - other.x) < eps && std::abs(y - other.y) < eps && std::abs(z - other.z) < eps;
    }
    [[nodiscard]] bool operator!=(const SVector3<T>& other) const noexcept
    {
        return !(*this==other);
    }
    friend std::ostream& operator<<(std::ostream& os, const SVector3<T>& vec)
    {
        os << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
        return os;
    }

SVector3<T>& operator=(const SVector3<T>& other) noexcept
{
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

    [[nodiscard]] T dot(const SVector3<T>& other) const noexcept
    {
        return *this*other;
    }

    [[nodiscard]] T cross2D(const SVector3<T>& other) const noexcept {
        return x*other.y - y*other.x;
    }

    [[nodiscard]] SVector3<T> cross4D(const SVector3<T>& other) const noexcept {
        return SVector3<T>(
        {
            y*other.z - z*other.y,
            z*other.x - x*other.z,
            x*other.y - y*other.x,
        });
    }

    [[nodiscard]] SVector3<T> cross3D(const SVector3<T>& other) const noexcept {
        return SVector3<T>(
            y*other.z - z*other.y,
            z*other.x - x*other.z,
            x*other.y - y*other.x);
    }

    [[nodiscard]] SVector3<T> normalize() const noexcept
    {
        const T len = std::sqrt(x*x + y*y + z*z);
        if (len == static_cast<T>(0)) {
            return SVector3<T>();
        }
        SVector3<T> ret = {x / len, y / len, z/ len};
        return ret;
    }

    void normalizeSelf() noexcept
    {
        const T len = std::sqrt(x*x + y*y + z*z);
        if (len == static_cast<T>(0)) {
            x = y = z = T{0};
            return;
        }
        x = x / len;
        y = y / len;
        z = z / len;
    }
    [[nodiscard]] T length() const noexcept
    {
        return std::sqrt(x*x + y*y + z*z);
    }
    [[nodiscard]] T lengthSquared() const noexcept
    {
        return x*x + y*y + z*z;
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
    [[nodiscard]] T distance(const SVector3<T>& other) const noexcept
    {
        return std::sqrt(distanceSquared(other));
    }
    [[nodiscard]] T distanceSquared(const SVector3<T>& other) const noexcept
    {
        T dx = x - other.x;
        T dy = y - other.y;
        T dz = z - other.z;
        return dx * dx + dy * dy + dz * dz;
    }
    [[nodiscard]] SVector3<T> lerp(const SVector3<T>& other, T t) const noexcept
    {
        return {
            x + (other.x - x) * t,
            y + (other.y - y) * t,
            z + (other.z - z) * t,
        };
    }
    [[nodiscard]] SVector3<T> reflect(const SVector3<T>& normal) const noexcept
    {
        T dotProduct = this->dot(normal);
        return {
            x - 2 * dotProduct * normal.x,
            y - 2 * dotProduct * normal.y,
            z - 2 * dotProduct * normal.z,
        };
    }
    [[nodiscard]] SVector3<T> project(const SVector3<T>& normal) const noexcept
    {
        T dotProduct = this->dot(normal);
        T normalLengthSquared = normal.lengthSquared();
        return {
            (dotProduct / normalLengthSquared) * normal.x,
            (dotProduct / normalLengthSquared) * normal.y,
            (dotProduct / normalLengthSquared) * normal.z,
        };
    }
    // Component-wise trig (angles in radians)
    [[nodiscard]] SVector3<T> sin() const noexcept
    {
        static_assert(std::is_floating_point_v<T>, "SVector3::sin requires floating-point T");
        return {
            static_cast<T>(std::sin(static_cast<double>(x))),
            static_cast<T>(std::sin(static_cast<double>(y))),
            static_cast<T>(std::sin(static_cast<double>(z)))
        };
    }
    [[nodiscard]] SVector3<T> cos() const noexcept
    {
        static_assert(std::is_floating_point_v<T>, "SVector3::cos requires floating-point T");
        return {
            static_cast<T>(std::cos(static_cast<double>(x))),
            static_cast<T>(std::cos(static_cast<double>(y))),
            static_cast<T>(std::cos(static_cast<double>(z)))
        };
    }
    [[nodiscard]] SVector3<T> tan() const noexcept
    {
        static_assert(std::is_floating_point_v<T>, "SVector3::tan requires floating-point T");
        return {
            static_cast<T>(std::tan(static_cast<double>(x))),
            static_cast<T>(std::tan(static_cast<double>(y))),
            static_cast<T>(std::tan(static_cast<double>(z)))
        };
    }
    [[nodiscard]] SVector3<T> asin() const noexcept
    {
        static_assert(std::is_floating_point_v<T>, "SVector3::asin requires floating-point T");
        T cx = x; if (cx < static_cast<T>(-1)) cx = static_cast<T>(-1); else if (cx > static_cast<T>(1)) cx = static_cast<T>(1);
        T cy = y; if (cy < static_cast<T>(-1)) cy = static_cast<T>(-1); else if (cy > static_cast<T>(1)) cy = static_cast<T>(1);
        T cz = z; if (cz < static_cast<T>(-1)) cz = static_cast<T>(-1); else if (cz > static_cast<T>(1)) cz = static_cast<T>(1);
        return {
            static_cast<T>(std::asin(static_cast<double>(cx))),
            static_cast<T>(std::asin(static_cast<double>(cy))),
            static_cast<T>(std::asin(static_cast<double>(cz)))
        };
    }
    [[nodiscard]] SVector3<T> acos() const noexcept
    {
        static_assert(std::is_floating_point_v<T>, "SVector3::acos requires floating-point T");
        T cx = x; if (cx < static_cast<T>(-1)) cx = static_cast<T>(-1); else if (cx > static_cast<T>(1)) cx = static_cast<T>(1);
        T cy = y; if (cy < static_cast<T>(-1)) cy = static_cast<T>(-1); else if (cy > static_cast<T>(1)) cy = static_cast<T>(1);
        T cz = z; if (cz < static_cast<T>(-1)) cz = static_cast<T>(-1); else if (cz > static_cast<T>(1)) cz = static_cast<T>(1);
        return {
            static_cast<T>(std::acos(static_cast<double>(cx))),
            static_cast<T>(std::acos(static_cast<double>(cy))),
            static_cast<T>(std::acos(static_cast<double>(cz)))
        };
    }
    [[nodiscard]] SVector3<T> atan() const noexcept
    {
        static_assert(std::is_floating_point_v<T>, "SVector3::atan requires floating-point T");
        return {
            static_cast<T>(std::atan(static_cast<double>(x))),
            static_cast<T>(std::atan(static_cast<double>(y))),
            static_cast<T>(std::atan(static_cast<double>(z)))
        };
    }
    // Degrees/radians helpers
    [[nodiscard]] SVector3<T> toRadians() const noexcept
    {
        static_assert(std::is_floating_point_v<T>, "SVector3::toRadians requires floating-point T");
        constexpr T pi = static_cast<T>(3.141592653589793238462643383279502884L);
        const T k = pi / static_cast<T>(180);
        return { x * k, y * k, z * k };
    }
    [[nodiscard]] SVector3<T> toDegrees() const noexcept
    {
        static_assert(std::is_floating_point_v<T>, "SVector3::toDegrees requires floating-point T");
        constexpr T pi = static_cast<T>(3.141592653589793238462643383279502884L);
        const T k = static_cast<T>(180) / pi;
        return { x * k, y * k, z * k };
    }
public:
    T x;
    T y;
    T z;
    // Alias reference members
    // Color aliases
    T& r; // -> x
    T& g; // -> y
    T& b; // -> z
    // Texture coordinate aliases
    T& s; // -> x
    T& t; // -> y
    T& p; // -> z (GLSL 3D texture coords)
    // Alternative texture aliases
    T& u; // -> x
    T& v; // -> y
    T& w; // -> z (note: in vec3 this is an alias, not a 4th component)
};
}

#endif
