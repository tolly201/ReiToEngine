#ifndef CORE_MATH_SVector_H
#define CORE_MATH_SVector_H

#include <cstdint>
#include <stdexcept> // For std::out_of_range
#include <vector>    // For std::vector
#include <initializer_list>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <type_traits>
#include <new>
#include "L20_Platform/L23_SingletonFactory/SingletonFactory.h"

namespace ReiToEngine
{
template <uint8_t DIM, typename T>
class SVector
{
public:
    static constexpr uint8_t Dimension = DIM;
    // Custom memory management
    static void* operator new(std::size_t sz)
    {
        return GetMemoryManager().Allocate(sz, static_cast<u8>(alignof(SVector)), RT_MEMORY_TAG::MATH);
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
        return GetMemoryManager().Allocate(sz, static_cast<u8>(alignof(SVector)), RT_MEMORY_TAG::MATH);
    }
    static void operator delete[](void* p, std::size_t sz) noexcept
    {
        if (p) GetMemoryManager().Free(p, sz, RT_MEMORY_TAG::MATH);
    }
    SVector(const std::initializer_list<T>& init) :
        x(data[0]),
        y(DIM >= 2 ? data[1] : data[0]),
        z(DIM >= 3 ? data[2] : data[0]),
        w(DIM >= 4 ? data[3] : data[0])
    {
        size_t list_size = init.size();
        list_size = list_size > DIM ? DIM : list_size;
        size_t i = 0;
        for (; i < list_size; ++i)
        {
            data[i] = init.begin()[i];
        }
        for (; i < DIM; ++i)
        {
            data[i] = T{0};
        }
    }
    SVector() :
        x(data[0]),
        y(DIM >= 2 ? data[1] : data[0]),
        z(DIM >= 3 ? data[2] : data[0]),
        w(DIM >= 4 ? data[3] : data[0])
    {
        size_t i = 0;
        for (; i < DIM; ++i)
        {
            data[i] = T{0};
        }
    }
    SVector(T val) :
        x(data[0]),
        y(DIM >= 2 ? data[1] : data[0]),
        z(DIM >= 3 ? data[2] : data[0]),
        w(DIM >= 4 ? data[3] : data[0])
    {
        size_t i = 0;
        for (; i < DIM; ++i)
        {
            data[i] = val;
        }
    }
    SVector(const SVector<DIM, T>& other) :
        x(data[0]),
        y(DIM >= 2 ? data[1] : data[0]),
        z(DIM >= 3 ? data[2] : data[0]),
        w(DIM >= 4 ? data[3] : data[0])
    {
        size_t i = 0;
        for (; i < DIM; ++i)
        {
            data[i] = other.data[i];
        }
    }

    // Move constructor is now deleted.  It doesn't make sense for array in class.
    // SVector(SVector<DIM, T>&& other) : data(other.data),
    //     x(data[0]),
    //     y(DIM >= 2 ? data[1] : data[0]),
    //     z(DIM >= 3 ? data[2] : data[0]),
    //     w(DIM >= 4 ? data[3] : data[0])
    // {
    //     //other.data = nullptr;  // No longer needed
    // }

    // Element functions from IVector (CRTP style, returning SVector<T, DIM>& and SVector<T, DIM>)
    SVector<DIM, T>& operator+=(const SVector<DIM, T>& other)
    {
        size_t i = 0;
        for (; i < DIM; ++i)
        {
            data[i] += other.data[i];
        }
        return *this;
    }
    SVector<DIM, T>& operator-=(const SVector<DIM, T>& other)
    {
        size_t i = 0;
        for (; i < DIM; ++i)
        {
            data[i] -= other.data[i];
        }
        return *this;
    }
    SVector<DIM, T>& operator*=(T scalar)
    {
        size_t i = 0;
        for (; i < DIM; ++i)
        {
            data[i] *= scalar;
        }
        return *this;
    }
    SVector<DIM, T>& operator/=(T scalar)
    {
        size_t i = 0;
        for (; i < DIM; ++i)
        {
            data[i] /= scalar;
        }
        return *this;
    }
    [[nodiscard]] SVector<DIM, T> operator+(const SVector<DIM, T>& other) const
    {
        SVector<DIM, T> result = *this;
        return result += other;
    }
    [[nodiscard]] SVector<DIM, T> operator-(const SVector<DIM, T>& other) const
    {
        SVector<DIM, T> result = *this;
        return result -= other;
    }
    [[nodiscard]] SVector<DIM, T> operator*(T scalar) const
    {
        SVector<DIM, T> result = *this;
        return result *= scalar;
    }
    [[nodiscard]] SVector<DIM, T> operator/(T scalar) const
    {
        SVector<DIM, T> result = *this;
        return result /= scalar;
    }
    [[nodiscard]] T operator*(const SVector<DIM, T>& other) const {
        T ret = T{};
        size_t i = 0;
        for (; i < DIM; ++i)
        {
            ret += data[i] * other.data[i];
        }
        return ret;
    }

    T& operator[](int index) {
        if (index < 0 || index > DIM - 1) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index > DIM - 1) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    bool operator==(const SVector<DIM, T>& other) const
    {
        const T eps = static_cast<T>(RT_COMPARE_PRECISION);
        for (size_t i = 0; i < DIM; ++i)
        {
            if (std::abs(data[i] - other.data[i]) > eps) return false;
        }
        return true;
    }
    bool operator!=(const SVector<DIM, T>& other) const
    {
        return !(*this == other);
    }
    friend std::ostream& operator<<(std::ostream& os, const SVector<DIM, T>& vec)
    {

        os << "(";
        size_t i = 0;
        for (; i < DIM - 1; ++i)
        {
            os << vec.data[i] << ", ";
        }
        os << vec.data[DIM - 1] << ")" << std::endl;
        return os;
    }

    SVector<DIM, T>& operator=(const SVector<DIM, T>& other)
    {
        size_t i = 0;
        for (; i < DIM; ++i)
        {
            data[i] = other.data[i];
        }
        return *this;
    }

    [[nodiscard]] T dot(const SVector<DIM, T>& other) const
    {
        return *this * other;
    }

    [[nodiscard]] T cross2D(const SVector<DIM, T>& other) const
    {
        assert(DIM >= 2 && "Call cross2D with a wrong DIMENTION.\n");
        return data[0] * other.data[1] - data[1] * other.data[0];
    }

    [[nodiscard]] SVector<DIM, T> cross4D(const SVector<DIM, T>& other) const {
        assert((DIM == 4 || DIM == 3) && "Call cross4D with a wrong DIMENTION.\n");
        if (DIM == 3) return cross3D(other);

        SVector<DIM, T> ret;
        ret[0] = data[1] * other.data[2] - data[2] * other.data[1] + data[3] * other.data[0] - data[0] * other.data[3];
        ret[1] = data[2] * other.data[0] - data[0] * other.data[2] + data[3] * other.data[1] - data[1] * other.data[3];
        ret[2] = data[0] * other.data[1] - data[1] * other.data[0] + data[3] * other.data[2] - data[2] * other.data[3];
        ret[3] = data[0] * other.data[3] - data[3] * other.data[0] + data[1] * other.data[2] - data[2] * other.data[1];
        return ret;
    }

    [[nodiscard]] SVector<DIM, T> cross3D(const SVector<DIM, T>& other) const {
        assert((DIM == 4 || DIM == 3) && "Call cross3D with a wrong DIMENTION.\n");

        SVector<DIM, T> ret;
        ret[0] = data[1] * other.data[2] - data[2] * other.data[1];
        ret[1] = data[2] * other.data[0] - data[0] * other.data[2];
        ret[2] = data[0] * other.data[1] - data[1] * other.data[0];
        return ret;
    }

    [[nodiscard]] SVector<DIM, T> normalize() const
    {
        T _length = length();
        if (_length == static_cast<T>(0)) {
            return SVector<DIM, T>();
        }
        SVector<DIM, T> ret = SVector();
        for (size_t i = 0; i < DIM; ++i)
        {
            ret.data[i] = data[i] / _length;
        }

        return ret;
    }

    void normalizeSelf()
    {
        T _length = length();
        if (_length == static_cast<T>(0)) {
            for (size_t i = 0; i < DIM; ++i)
            {
                data[i] = 0;
            }
            return;
        }
        for (size_t i = 0; i < DIM; ++i)
        {
            data[i] = data[i] / _length;
        }
    }
    [[nodiscard]] T length() const
    {
        return std::sqrt(lengthSquared());
    }
    [[nodiscard]] T lengthSquared() const
    {
        T ret = T{0};
        for (size_t i = 0; i < DIM; ++i)
        {
            ret += data[i] * data[i];
        }
        return ret;
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
    [[nodiscard]] T distance(const SVector<DIM, T>& other) const
    {
        return std::sqrt(distanceSquared(other));
    }
    [[nodiscard]] T distanceSquared(const SVector<DIM, T>& other) const
    {
        T ret = T{0};
        for (size_t i = 0; i < DIM; ++i)
        {
            ret += (data[i] - other.data[i]) * (data[i] - other.data[i]);
        }
        return ret;
    }
    [[nodiscard]] SVector<DIM, T> lerp(const SVector<DIM, T>& other, T t) const
    {
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i)
        {
            ret.data[i] = data[i] + (other.data[i] - data[i]) * t;
        }
        return ret;
    }
    [[nodiscard]] SVector<DIM, T> reflect(const SVector<DIM, T>& normal) const
    {
        T dotProduct = this->dot(normal);

        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i)
        {
            ret.data[i] = data[i] - 2 * dotProduct * normal.data[i];
        }
        return ret;
    }
    [[nodiscard]] SVector<DIM, T> project(const SVector<DIM, T>& normal) const
    {
        T dotProduct = this->dot(normal);
        T normalLengthSquared = normal.lengthSquared();
        T mult = dotProduct / normalLengthSquared;
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i)
        {
            ret.data[i] = mult * normal.data[i];
        }
        return ret;
    }
    // Component-wise trig functions (angles in radians)
    [[nodiscard]] SVector<DIM, T> sin() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector::sin requires floating-point T");
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i) ret.data[i] = static_cast<T>(std::sin(static_cast<double>(data[i])));
        return ret;
    }
    [[nodiscard]] SVector<DIM, T> cos() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector::cos requires floating-point T");
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i) ret.data[i] = static_cast<T>(std::cos(static_cast<double>(data[i])));
        return ret;
    }
    [[nodiscard]] SVector<DIM, T> tan() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector::tan requires floating-point T");
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i) ret.data[i] = static_cast<T>(std::tan(static_cast<double>(data[i])));
        return ret;
    }
    // Inverse trig with clamping to [-1, 1]
    [[nodiscard]] SVector<DIM, T> asin() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector::asin requires floating-point T");
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i) {
            T v = data[i];
            if (v < static_cast<T>(-1)) v = static_cast<T>(-1); else if (v > static_cast<T>(1)) v = static_cast<T>(1);
            ret.data[i] = static_cast<T>(std::asin(static_cast<double>(v)));
        }
        return ret;
    }
    [[nodiscard]] SVector<DIM, T> acos() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector::acos requires floating-point T");
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i) {
            T v = data[i];
            if (v < static_cast<T>(-1)) v = static_cast<T>(-1); else if (v > static_cast<T>(1)) v = static_cast<T>(1);
            ret.data[i] = static_cast<T>(std::acos(static_cast<double>(v)));
        }
        return ret;
    }
    [[nodiscard]] SVector<DIM, T> atan() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector::atan requires floating-point T");
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i) ret.data[i] = static_cast<T>(std::atan(static_cast<double>(data[i])));
        return ret;
    }
    // Degree/radian component-wise conversion helpers
    [[nodiscard]] SVector<DIM, T> toRadians() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector::toRadians requires floating-point T");
        constexpr T pi = static_cast<T>(3.141592653589793238462643383279502884L);
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i) ret.data[i] = data[i] * (pi / static_cast<T>(180));
        return ret;
    }
    [[nodiscard]] SVector<DIM, T> toDegrees() const
    {
        static_assert(std::is_floating_point_v<T>, "SVector::toDegrees requires floating-point T");
        constexpr T pi = static_cast<T>(3.141592653589793238462643383279502884L);
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i) ret.data[i] = data[i] * (static_cast<T>(180) / pi);
        return ret;
    }
    // Element-wise product
    [[nodiscard]] SVector<DIM, T> hadamard(const SVector<DIM, T>& other) const
    {
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i) ret.data[i] = data[i] * other.data[i];
        return ret;
    }
    // Clamp each component into [minV, maxV]
    [[nodiscard]] SVector<DIM, T> clamp(T minV, T maxV) const
    {
        SVector<DIM, T> ret;
        for (size_t i = 0; i < DIM; ++i)
        {
            T v = data[i];
            if (v < minV) v = minV; else if (v > maxV) v = maxV;
            ret.data[i] = v;
        }
        return ret;
    }
    // Angle between vectors (in radians); returns 0 for zero-length inputs
    [[nodiscard]] T angle(const SVector<DIM, T>& other) const
    {
        T ls = this->lengthSquared();
        T rs = other.lengthSquared();
        if (ls <= static_cast<T>(RT_COMPARE_PRECISION) || rs <= static_cast<T>(RT_COMPARE_PRECISION)) return static_cast<T>(0);
        T c = (*this * other) / (std::sqrt(ls) * std::sqrt(rs));
        if (c < static_cast<T>(-1)) c = static_cast<T>(-1);
        if (c > static_cast<T>(1))  c = static_cast<T>(1);
        return std::acos(c);
    }
    [[nodiscard]] bool approxEqual(const SVector<DIM, T>& other, T eps = static_cast<T>(RT_COMPARE_PRECISION)) const
    {
        for (size_t i = 0; i < DIM; ++i)
        {
            if (std::abs(data[i] - other.data[i]) > eps) return false;
        }
        return true;
    }
    [[nodiscard]] bool isFinite() const
    {
        for (size_t i = 0; i < DIM; ++i)
        {
            if (!std::isfinite(static_cast<double>(data[i]))) return false;
        }
        return true;
    }
public:
    T data[DIM];
    T& x;
    T& y;
    T& z;
    T& w;
};
}

#endif
