#ifndef CORE_MATH_SVector2_H
#define CORE_MATH_SVector2_H
#include "IVector.h"
#include <initializer_list>
#include <cmath>
#include "L20_Platform/L23_Logger/Include.h"
namespace ReiToEngine
{
template <typename T>
class SVector2 final : public IVector<SVector2<T>, 2, T>
{
public:
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
    SVector2<T>& operator+=(const SVector2<T>& other) noexcept override
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    SVector2<T>& operator-=(const SVector2<T>& other) noexcept override
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    SVector2<T>& operator*=(T scalar) noexcept override
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    SVector2<T>& operator/=(T scalar) noexcept override
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    SVector2<T> operator+(const SVector2<T>& other) const noexcept override
    {
        return SVector2<T>{x + other.x, y + other.y};
    }
    SVector2<T> operator-(const SVector2<T>& other) const noexcept override
    {
        return SVector2<T>{x - other.x, y - other.y};
    }
    SVector2<T> operator*(T scalar) const noexcept override
    {
        return SVector2<T>{x * scalar, y * scalar};
    }
    SVector2<T> operator/(T scalar) const noexcept override
    {
        return SVector2<T>{x / scalar, y / scalar};
    }
    T operator*(const SVector2<T>& other) const noexcept override {
        return x*other.x + y*other.y;
    }

    T& operator[](int index) noexcept override {
        RT_ASSERT(index >= 0 && index < 2);
        return index == 0 ? x : y;
    }

    const T& operator[](int index) const noexcept override {
        RT_ASSERT(index >= 0 && index < 2);
        return index == 0 ? x : y;
    }

    bool operator==(const SVector2<T>& other) const noexcept override
    {
        const T eps = static_cast<T>(RT_COMPARE_PRECISION);
        return std::abs(x - other.x) < eps && std::abs(y - other.y) < eps;
    }
    bool operator!=(const SVector2<T>& other) const noexcept override
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

    T dot(const SVector2<T>& other) const noexcept override
    {
        return *this*other;
    }

    T cross2D(const SVector2<T>& other) const noexcept override
    {
        return x*other.y - y*other.x;
    }

    SVector2<T> cross4D(const SVector2<T>& other) const noexcept override {
        return SVector2<T>();
    }

    SVector2<T> cross3D(const SVector2<T>& other) const noexcept override {
        return SVector2<T>(x*other.y - y*other.x, T{0});
    }

    SVector2<T> normalize() const noexcept override
    {
        const T len = std::sqrt(x*x + y*y);
        if (len == static_cast<T>(0)) {
            return SVector2<T>();
        }
        SVector2<T> ret = {x / len, y / len};
        return ret;
    }

    void normalizeSelf() noexcept override
    {
        const T len = std::sqrt(x*x + y*y);
        if (len == static_cast<T>(0)) {
            x = y = T{0};
            return;
        }
        x = x / len;
        y = y / len;
    }
    T length() const noexcept override
    {
        return std::sqrt(x*x + y*y);
    }
    T lengthSquared() const noexcept override
    {
        return x*x + y*y;
    }
    bool isZero() const noexcept override
    {
        const T eps = static_cast<T>(RT_COMPARE_PRECISION);
        return this->lengthSquared() < eps;
    }
    bool isNormalized() const noexcept override
    {
        const T eps = static_cast<T>(RT_COMPARE_PRECISION);
        return std::abs(this->lengthSquared() - static_cast<T>(1)) < eps;
    }
    T distance(const SVector2<T>& other) const noexcept override
    {
        return std::sqrt(distanceSquared(other));
    }
    T distanceSquared(const SVector2<T>& other) const noexcept override
    {
        T dx = x - other.x;
        T dy = y - other.y;
        return dx * dx + dy * dy;
    }
    SVector2<T> lerp(const SVector2<T>& other, T t) const noexcept override
    {
        return {
            x + (other.x - x) * t,
            y + (other.y - y) * t,
            };
    }
    SVector2<T> reflect(const SVector2<T>& normal) const noexcept override
    {
        T dotProduct = this->dot(normal);
        return SVector2<T>{
            x - 2 * dotProduct * normal.x,
            y - 2 * dotProduct * normal.y,
        };
    }
    SVector2<T> project(const SVector2<T>& normal) const noexcept override
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
