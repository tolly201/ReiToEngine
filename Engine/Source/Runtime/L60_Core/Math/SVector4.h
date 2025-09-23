#ifndef CORE_MATH_SVector4_H
#define CORE_MATH_SVector4_H
#include <cstdint>
#include <stdexcept> // For std::out_of_range
#include <vector>    // For std::vector
#include <initializer_list>
#include <algorithm>
#include <cmath>
#include <type_traits>
#include <new>
#include "L20_Platform/L31_SingletonFactory/SingletonFactory.h"

namespace ReiToEngine
{
template <typename T>
class alignas((std::is_same_v<T, float>) ? 16 : alignof(T)) SVector4
{
public:
    static constexpr uint8_t Dimension = 4;
    // Custom memory management
    static void* operator new(std::size_t sz)
    {
        return GetMemoryManager().Allocate(sz, static_cast<u8>(alignof(SVector4)), RT_MEMORY_TAG::MATH);
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
        return GetMemoryManager().Allocate(sz, static_cast<u8>(alignof(SVector4)), RT_MEMORY_TAG::MATH);
    }
    static void operator delete[](void* p, std::size_t sz) noexcept
    {
        if (p) GetMemoryManager().Free(p, sz, RT_MEMORY_TAG::MATH);
    }
    SVector4(const std::initializer_list<T>& init) noexcept
        : x(T{0}), y(T{0}), z(T{0}), w(T{0})
        , r(x), g(y), b(z), a(w)
        , s(x), t(y), p(z), q(w)
    {
        int list_size = static_cast<int>(init.size());
        list_size = list_size > 4 ? 4 : list_size;
        if (list_size > 0) x = init.begin()[0];
        if (list_size > 1) y = init.begin()[1];
        if (list_size > 2) z = init.begin()[2];
        if (list_size > 3) w = init.begin()[3];
    }
    SVector4() noexcept
        : x(T{0}), y(T{0}), z(T{0}), w(T{0})
        , r(x), g(y), b(z), a(w)
        , s(x), t(y), p(z), q(w)
    {}
    SVector4(T x, T y, T z, T w) noexcept
        : x(x), y(y), z(z), w(w)
        , r(this->x), g(this->y), b(this->z), a(this->w)
        , s(this->x), t(this->y), p(this->z), q(this->w)
    {};
    SVector4(T val) noexcept
        : x(val), y(val), z(val), w(val)
        , r(this->x), g(this->y), b(this->z), a(this->w)
        , s(this->x), t(this->y), p(this->z), q(this->w)
    {};
    SVector4(const SVector4<T>& other)
        : x(other.x), y(other.y), z(other.z), w(other.w)
        , r(this->x), g(this->y), b(this->z), a(this->w)
        , s(this->x), t(this->y), p(this->z), q(this->w)
    {}
    SVector4(SVector4<T>&& other) noexcept
        : x(other.x), y(other.y), z(other.z), w(other.w)
        , r(this->x), g(this->y), b(this->z), a(this->w)
        , s(this->x), t(this->y), p(this->z), q(this->w)
    {}
    // SVector4<T>ement functions from IVector (CRTP style, returning SVector<T, DIM>& and SVector<T, DIM>)
    SVector4<T>& operator+=(const SVector4<T>& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    SVector4<T>& operator-=(const SVector4<T>& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    SVector4<T>& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }
    SVector4<T>& operator/=(T scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }
    [[nodiscard]] SVector4<T> operator+(const SVector4<T>& other) const noexcept
    {
        SVector4<T> result = *this;
        return result += other;
    }
    [[nodiscard]] SVector4<T> operator-(const SVector4<T>& other) const noexcept
    {
        SVector4<T> result = *this;
        return result -= other;
    }
    [[nodiscard]] SVector4<T> operator*(T scalar) const noexcept
    {
        SVector4<T> result = *this;
        return result *= scalar;
    }
    [[nodiscard]] SVector4<T> operator/(T scalar) const noexcept
    {
        SVector4<T> result = *this;
        return result /= scalar;
    }
    [[nodiscard]] T operator*(const SVector4<T>& other) const noexcept {
        return x*other.x + y*other.y + z*other.z + w*other.w;
    }

    T& operator[](int index) {
        if (index < 0 || index > 3) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        if (index == 3) return w;
        return x;
    }

    [[nodiscard]] const T& operator[](int index) const {
        if (index < 0 || index > 3) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        if (index == 3) return w;
        return x;
    }

    [[nodiscard]] bool operator==(const SVector4<T>& other) const
    {
        return std::abs(x - other.x) < RT_COMPARE_PRECISION &&
           std::abs(y - other.y) < RT_COMPARE_PRECISION &&
           std::abs(z - other.z) < RT_COMPARE_PRECISION &&
           std::abs(w - other.w) < RT_COMPARE_PRECISION;
    }
    [[nodiscard]] bool operator!=(const SVector4<T>& other) const
    {
        return !(*this==other);
    }
    friend std::ostream& operator<<(std::ostream& os, const SVector4<T>& vec)
    {
        os << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")";
        return os;
    }

SVector4<T>& operator=(const SVector4<T>& other)
{
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
    return *this;
}

    [[nodiscard]] T dot(const SVector4<T>& other) const
    {
        return *this*other;
    }

    [[nodiscard]] T cross2D(const SVector4<T>& other) const {
        return x*other.y - y*other.x;
    }

    [[nodiscard]] SVector4<T> cross4D(const SVector4<T>& other) const {
        return SVector4<T>
        (
            y*other.z - z*other.y + w*other.x - x*other.w,
            z*other.x - x*other.z + w*other.y - y*other.w,
            x*other.y - y*other.x + w*other.z - z*other.w,
            x*other.w - w*other.x + y*other.z - z*other.y
        );
    }

    [[nodiscard]] SVector4<T> cross3D(const SVector4<T>& other) const {
        return SVector4<T>(
            y*other.z - z*other.y,
            z*other.x - x*other.z,
            x*other.y - y*other.x,
            T{0}
        );
    }

    [[nodiscard]] SVector4<T> normalize() const
    {
        T length = std::sqrt(x*x + y*y + z*z + w*w);
        if (length == static_cast<T>(0)) {
            return SVector4<T>();
        }
        SVector4<T> ret = {x / length, y / length, z/ length, w/length};
        return ret;
    }

    void normalizeSelf()
    {
        T length = std::sqrt(x*x + y*y + z*z + w*w);
        if (length == static_cast<T>(0)) {
            x = y = z = w = T{0};
            return;
        }
        x = x / length;
        y = y / length;
        z = z / length;
        w = w / length;
    }
    [[nodiscard]] T length() const
    {
        return std::sqrt(x*x + y*y + z*z + w*w);
    }
    [[nodiscard]] T lengthSquared() const
    {
        return x*x + y*y + z*z + w*w;
    }
    [[nodiscard]] bool isZero() const
    {
        return this->lengthSquared() < RT_COMPARE_PRECISION;
    }
    [[nodiscard]] bool isNormalized() const
    {
        const T eps = static_cast<T>(RT_COMPARE_PRECISION);
        return std::abs(this->lengthSquared() - static_cast<T>(1)) < eps;
    }
    [[nodiscard]] T distance(const SVector4<T>& other) const
    {
        return std::sqrt(distanceSquared(other));
    }
    [[nodiscard]] T distanceSquared(const SVector4<T>& other) const
    {
        T dx = x - other.x;
        T dy = y - other.y;
        T dz = z - other.z;
        T dw = w - other.w;
        return dx * dx + dy * dy + dz * dz + dw * dw;
    }
    [[nodiscard]] SVector4<T> lerp(const SVector4<T>& other, T t) const
    {
        return SVector4<T>(
            x + (other.x - x) * t,
            y + (other.y - y) * t,
            z + (other.z - z) * t,
            w + (other.w - w) * t
        );
    }
    [[nodiscard]] SVector4<T> reflect(const SVector4<T>& normal) const
    {
        T dotProduct = this->dot(normal);
        return SVector4<T>(
            x - 2 * dotProduct * normal.x,
            y - 2 * dotProduct * normal.y,
            z - 2 * dotProduct * normal.z,
            w - 2 * dotProduct * normal.w
        );
    }
    [[nodiscard]] SVector4<T> project(const SVector4<T>& normal) const
    {
        T dotProduct = this->dot(normal);
        T normalLengthSquared = normal.lengthSquared();
        return SVector4<T>(
            (dotProduct / normalLengthSquared) * normal.x,
            (dotProduct / normalLengthSquared) * normal.y,
            (dotProduct / normalLengthSquared) * normal.z,
            (dotProduct / normalLengthSquared) * normal.w
        );
    }
    // Component-wise trig (angles in radians)
    [[nodiscard]] SVector4<T> sin() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector4::sin requires floating-point T");
        return SVector4<T>(
            static_cast<T>(std::sin(static_cast<double>(x))),
            static_cast<T>(std::sin(static_cast<double>(y))),
            static_cast<T>(std::sin(static_cast<double>(z))),
            static_cast<T>(std::sin(static_cast<double>(w)))
        );
    }
    [[nodiscard]] SVector4<T> cos() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector4::cos requires floating-point T");
        return SVector4<T>(
            static_cast<T>(std::cos(static_cast<double>(x))),
            static_cast<T>(std::cos(static_cast<double>(y))),
            static_cast<T>(std::cos(static_cast<double>(z))),
            static_cast<T>(std::cos(static_cast<double>(w)))
        );
    }
    [[nodiscard]] SVector4<T> tan() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector4::tan requires floating-point T");
        return SVector4<T>(
            static_cast<T>(std::tan(static_cast<double>(x))),
            static_cast<T>(std::tan(static_cast<double>(y))),
            static_cast<T>(std::tan(static_cast<double>(z))),
            static_cast<T>(std::tan(static_cast<double>(w)))
        );
    }
    [[nodiscard]] SVector4<T> asin() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector4::asin requires floating-point T");
        T cx = x; if (cx < static_cast<T>(-1)) cx = static_cast<T>(-1); else if (cx > static_cast<T>(1)) cx = static_cast<T>(1);
        T cy = y; if (cy < static_cast<T>(-1)) cy = static_cast<T>(-1); else if (cy > static_cast<T>(1)) cy = static_cast<T>(1);
        T cz = z; if (cz < static_cast<T>(-1)) cz = static_cast<T>(-1); else if (cz > static_cast<T>(1)) cz = static_cast<T>(1);
        T cw = w; if (cw < static_cast<T>(-1)) cw = static_cast<T>(-1); else if (cw > static_cast<T>(1)) cw = static_cast<T>(1);
        return SVector4<T>(
            static_cast<T>(std::asin(static_cast<double>(cx))),
            static_cast<T>(std::asin(static_cast<double>(cy))),
            static_cast<T>(std::asin(static_cast<double>(cz))),
            static_cast<T>(std::asin(static_cast<double>(cw)))
        );
    }
    [[nodiscard]] SVector4<T> acos() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector4::acos requires floating-point T");
        T cx = x; if (cx < static_cast<T>(-1)) cx = static_cast<T>(-1); else if (cx > static_cast<T>(1)) cx = static_cast<T>(1);
        T cy = y; if (cy < static_cast<T>(-1)) cy = static_cast<T>(-1); else if (cy > static_cast<T>(1)) cy = static_cast<T>(1);
        T cz = z; if (cz < static_cast<T>(-1)) cz = static_cast<T>(-1); else if (cz > static_cast<T>(1)) cz = static_cast<T>(1);
        T cw = w; if (cw < static_cast<T>(-1)) cw = static_cast<T>(-1); else if (cw > static_cast<T>(1)) cw = static_cast<T>(1);
        return SVector4<T>(
            static_cast<T>(std::acos(static_cast<double>(cx))),
            static_cast<T>(std::acos(static_cast<double>(cy))),
            static_cast<T>(std::acos(static_cast<double>(cz))),
            static_cast<T>(std::acos(static_cast<double>(cw)))
        );
    }
    [[nodiscard]] SVector4<T> atan() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector4::atan requires floating-point T");
        return SVector4<T>(
            static_cast<T>(std::atan(static_cast<double>(x))),
            static_cast<T>(std::atan(static_cast<double>(y))),
            static_cast<T>(std::atan(static_cast<double>(z))),
            static_cast<T>(std::atan(static_cast<double>(w)))
        );
    }
    // Degrees/radians helpers
    [[nodiscard]] SVector4<T> toRadians() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector4::toRadians requires floating-point T");
        constexpr T pi = static_cast<T>(3.141592653589793238462643383279502884L);
        const T k = pi / static_cast<T>(180);
        return SVector4<T>(x * k, y * k, z * k, w * k);
    }
    [[nodiscard]] SVector4<T> toDegrees() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector4::toDegrees requires floating-point T");
        constexpr T pi = static_cast<T>(3.141592653589793238462643383279502884L);
        const T k = static_cast<T>(180) / pi;
        return SVector4<T>(x * k, y * k, z * k, w * k);
    }
public:
    T x;
    T y;
    T z;
    T w;
    // Alias reference members (colors and texture coords)
    // Color aliases
    T& r; // -> x
    T& g; // -> y
    T& b; // -> z
    T& a; // -> w
    // Texture coordinate aliases
    T& s; // -> x
    T& t; // -> y
    T& p; // -> z
    T& q; // -> w
};
}

#endif
