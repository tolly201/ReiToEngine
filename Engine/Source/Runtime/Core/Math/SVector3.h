#ifndef CORE_MATH_SVector3_H
#define CORE_MATH_SVector3_H
#include "IVector.h"
#include <stdexcept> // For std::out_of_range
#include <vector>    // For std::vector
#include <initializer_list>
#include <algorithm>
#include <cmath>

namespace ReiToEngine
{
template <typename T>
class SVector3 : public IVector<SVector3<T>, 3, T>
{
public:
    SVector3(const std::initializer_list<T>& init)
    {
        x = y = z = 0;
        int list_size = init.size();
        list_size = list_size > 3? 3 : list_size;
        if (list_size > 0) x = init.begin()[0];
        if (list_size > 1) y = init.begin()[1];
        if (list_size > 2) z = init.begin()[2];
    }
    SVector3() = default;
    SVector3(T x, T y, T z):
        x(x), y(y), z(z) {};
    SVector3(T val):x(val), y(val), z(val) {};
    SVector3(const SVector3<T>& other) = default;
    SVector3(SVector3<T>&&)noexcept = default;
    // SVector3<T>ement functions from IVector (CRTP style, returning SVector<T, DIM>& and SVector<T, DIM>)
    SVector3<T>& operator+=(const SVector3<T>& other) override
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    SVector3<T>& operator-=(const SVector3<T>& other) override
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    SVector3<T>& operator*=(T scalar) override
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    SVector3<T>& operator/=(T scalar) override
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    SVector3<T> operator+(const SVector3<T>& other) const override
    {
        SVector3<T> result = *this;
        return result += other;
    }
    SVector3<T> operator-(const SVector3<T>& other) const override
    {
        SVector3<T> result = *this;
        return result -= other;
    }
    SVector3<T> operator*(T scalar) const override
    {
        SVector3<T> result = *this;
        return result *= scalar;
    }
    SVector3<T> operator/(T scalar) const override
    {
        SVector3<T> result = *this;
        return result /= scalar;
    }
    T operator*(const SVector3<T>& other) const override {
        return x*other.x + y*other.y + z*other.z;
    }

    T& operator[](int index) override {
        if (index < 0 || index > 2) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        return x;
    }

    const T& operator[](int index) const override {
        if (index < 0 || index > 2) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        return x;
    }

    bool operator==(const SVector3<T>& other) const override
    {
        return std::abs(x - other.x) < RT_COMPARE_PRECISION &&
           std::abs(y - other.y) < RT_COMPARE_PRECISION &&
           std::abs(z - other.z) < RT_COMPARE_PRECISION;
    }
    bool operator!=(const SVector3<T>& other) const override
    {
        return !(*this==other);
    }
    friend std::ostream& operator<<(std::ostream& os, const SVector3<T>& vec)
    {
        os << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
        return os;
    }

SVector3<T>& operator=(const SVector3<T>& other)
{
    if (this != &other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

    T dot(const SVector3<T>& other) const override
    {
        return *this*other;
    }

    T cross2D(const SVector3<T>& other) const override {
        return 0;
    }

    SVector3<T> cross4D(const SVector3<T>& other) const override {
        return
        {
            y*other.z - z*other.y,
            z*other.x - x*other.z,
            x*other.y - y*other.x,
        };
    }

    SVector3<T> cross3D(const SVector3<T>& other) const override {
        return
        {
            y*other.z - z*other.y,
            z*other.x - x*other.z,
            x*other.y - y*other.x,
        };
    }

    SVector3<T> normalize() const override
    {
        T length = std::sqrt(x*x + y*y + z*z);
        if (length == static_cast<T>(0)) {
            return {0, 0, 0};
        }
        SVector3<T> ret = {x / length, y / length, z/ length};
        return ret;
    }

    void normalizeSelf() override
    {
        T length = std::sqrt(x*x + y*y + z*z);
        if (length == static_cast<T>(0)) {
            x = y = z = 0;
        }
        x = x / length;
        y = y / length;
        z = z / length;
    }
    T length() const override
    {
        return std::sqrt(x*x + y*y + z*z);
    }
    T lengthSquared() const override
    {
        return x*x + y*y + z*z;
    }
    bool isZero() const override
    {
        return this->lengthSquared() < RT_COMPARE_PRECISION;
    }
    bool isNormalized() const override
    {
        return std::abs(this->lengthSquared() - 1.0f) < RT_COMPARE_PRECISION;
    }
    T distance(const SVector3<T>& other) const override
    {
        return std::sqrt(distanceSquared(other));
    }
    T distanceSquared(const SVector3<T>& other) const override
    {
        T dx = x - other.x;
        T dy = y - other.y;
        T dz = z - other.z;
        return dx * dx + dy * dy + dz * dz;
    }
    SVector3<T> lerp(const SVector3<T>& other, T t) const override
    {
        return {
            x + (other.x - x) * t,
            y + (other.y - y) * t,
            z + (other.z - z) * t,
        };
    }
    SVector3<T> reflect(const SVector3<T>& normal) const override
    {
        T dotProduct = this->dot(normal);
        return {
            x - 2 * dotProduct * normal.x,
            y - 2 * dotProduct * normal.y,
            z - 2 * dotProduct * normal.z,
        };
    }
    SVector3<T> project(const SVector3<T>& normal) const override
    {
        T dotProduct = this->dot(normal);
        T normalLengthSquared = normal.lengthSquared();
        return {
            (dotProduct / normalLengthSquared) * normal.x,
            (dotProduct / normalLengthSquared) * normal.y,
            (dotProduct / normalLengthSquared) * normal.z,
        };
    }
public:
    T x;
    T y;
    T z;
};
}

#endif
