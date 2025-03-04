#ifndef CORE_MATH_SVector2_H
#define CORE_MATH_SVector2_H
#include "IVector.h"
#include <stdexcept> // For std::out_of_range
#include <vector>    // For std::vector
#include <initializer_list>
#include <algorithm>
#include <cmath>

namespace ReiToEngine
{
template <typename T>
class SVector2 : public IVector<SVector2<T>, 2, T>
{
public:
    SVector2(const std::initializer_list<T>& init)
    {
        x = y = 0;
        int list_size = init.size();
        list_size = list_size > 2? 2 : list_size;
        if (list_size > 0) x = init.begin()[0];
        if (list_size > 1) y = init.begin()[1];
    }
    SVector2() = default;
    SVector2(T x, T y):
        x(x), y(y) {};
    SVector2(T val):x(val), y(val) {};
    SVector2(const SVector2<T>& other) = default;
    SVector2(SVector2<T>&&)noexcept = default;
    // SVector2<T>ement functions from IVector (CRTP style, returning SVector<T, DIM>& and SVector<T, DIM>)
    SVector2<T>& operator+=(const SVector2<T>& other) override
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    SVector2<T>& operator-=(const SVector2<T>& other) override
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    SVector2<T>& operator*=(T scalar) override
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    SVector2<T>& operator/=(T scalar) override
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    SVector2<T> operator+(const SVector2<T>& other) const override
    {
        SVector2<T> result = *this;
        return result += other;
    }
    SVector2<T> operator-(const SVector2<T>& other) const override
    {
        SVector2<T> result = *this;
        return result -= other;
    }
    SVector2<T> operator*(T scalar) const override
    {
        SVector2<T> result = *this;
        return result *= scalar;
    }
    SVector2<T> operator/(T scalar) const override
    {
        SVector2<T> result = *this;
        return result /= scalar;
    }
    T operator*(const SVector2<T>& other) const override {
        return x*other.x + y*other.y;
    }

    T& operator[](int index) override {
        if (index < 0 || index > 1) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return x;
        if (index == 1) return y;
        return x;
    }

    const T& operator[](int index) const override {
        if (index < 0 || index > 1) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return x;
        if (index == 1) return y;
        return x;
    }

    bool operator==(const SVector2<T>& other) const override
    {
        return std::abs(x - other.x) < RT_COMPARE_PRECISION &&
           std::abs(y - other.y) < RT_COMPARE_PRECISION;
    }
    bool operator!=(const SVector2<T>& other) const override
    {
        return !(*this==other);
    }
    friend std::ostream& operator<<(std::ostream& os, const SVector2<T>& vec)
    {
        os << "(" << vec[0] << ", " << vec[1] << ")";
        return os;
    }

SVector2<T>& operator=(const SVector2<T>& other)
{
    if (this != &other) {
        x = other.x;
        y = other.y;
    }
    return *this;
}

    T dot(const SVector2<T>& other) const override
    {
        return *this*other;
    }

    T cross2D(const SVector2<T>& other) const override
    {
        return x*other.y - y*other.x;
    }

    SVector2<T> cross4D(const SVector2<T>& other) const override {
        return SVector2<T>(0, 0);
    }

    SVector2<T> cross3D(const SVector2<T>& other) const override {
        return SVector2<T>(x*other.y - y*other.x, 0);
    }

    SVector2<T> normalize() const override
    {
        T length = std::sqrt(x*x + y*y);
        if (length == static_cast<T>(0)) {
            return {0, 0};
        }
        SVector2<T> ret = {x / length, y / length};
        return ret;
    }

    void normalizeSelf() override
    {
        T length = std::sqrt(x*x + y*y);
        if (length == static_cast<T>(0)) {
            x = y = 0;
        }
        x = x / length;
        y = y / length;
    }
    T length() const override
    {
        return std::sqrt(x*x + y*y);
    }
    T lengthSquared() const override
    {
        return x*x + y*y;
    }
    bool isZero() const override
    {
        return this->lengthSquared() < RT_COMPARE_PRECISION;
    }
    bool isNormalized() const override
    {
        return std::abs(this->lengthSquared() - 1.0f) < RT_COMPARE_PRECISION;
    }
    T distance(const SVector2<T>& other) const override
    {
        return std::sqrt(distanceSquared(other));
    }
    T distanceSquared(const SVector2<T>& other) const override
    {
        T dx = x - other.x;
        T dy = y - other.y;
        return dx * dx + dy * dy;
    }
    SVector2<T> lerp(const SVector2<T>& other, T t) const override
    {
        return {
            x + (other.x - x) * t,
            y + (other.y - y) * t,
            };
    }
    SVector2<T> reflect(const SVector2<T>& normal) const override
    {
        T dotProduct = this->dot(normal);
        return SVector2<T>{
            x - 2 * dotProduct * normal.x,
            y - 2 * dotProduct * normal.y,
        };
    }
    SVector2<T> project(const SVector2<T>& normal) const override
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
