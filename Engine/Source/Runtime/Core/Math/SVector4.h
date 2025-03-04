#ifndef CORE_MATH_SVector4_H
#define CORE_MATH_SVector4_H
#include "IVector.h"
#include <stdexcept> // For std::out_of_range
#include <vector>    // For std::vector
#include <initializer_list>
#include <algorithm>
#include <cmath>

namespace ReiToEngine
{
template <typename T>
class SVector4 : public IVector<SVector4<T>, 4, T>
{
public:
    SVector4(const std::initializer_list<T>& init)
    {
        x = y = z = w = 0;
        int list_size = init.size();
        list_size = list_size > 4? 4 : list_size;
        if (list_size > 0) x = init.begin()[0];
        if (list_size > 1) y = init.begin()[1];
        if (list_size > 2) z = init.begin()[2];
        if (list_size > 3) w = init.begin()[3];
    }
    SVector4() = default;
    SVector4(T x, T y, T z, T w):
        x(x), y(y), z(z), w(w) {};
    SVector4(T val):x(val), y(val), z(val), w(val) {};
    SVector4(const SVector4<T>& other) = default;
    SVector4(SVector4<T>&&)noexcept = default;
    // SVector4<T>ement functions from IVector (CRTP style, returning SVector<T, DIM>& and SVector<T, DIM>)
    SVector4<T>& operator+=(const SVector4<T>& other) override
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    SVector4<T>& operator-=(const SVector4<T>& other) override
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    SVector4<T>& operator*=(T scalar) override
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }
    SVector4<T>& operator/=(T scalar) override
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }
    SVector4<T> operator+(const SVector4<T>& other) const override
    {
        SVector4<T> result = *this;
        return result += other;
    }
    SVector4<T> operator-(const SVector4<T>& other) const override
    {
        SVector4<T> result = *this;
        return result -= other;
    }
    SVector4<T> operator*(T scalar) const override
    {
        SVector4<T> result = *this;
        return result *= scalar;
    }
    SVector4<T> operator/(T scalar) const override
    {
        SVector4<T> result = *this;
        return result /= scalar;
    }
    T operator*(const SVector4<T>& other) const override {
        return x*other.x + y*other.y + z*other.z + w*other.w;
    }

    T& operator[](int index) override {
        if (index < 0 || index > 3) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        if (index == 3) return w;
        return x;
    }

    const T& operator[](int index) const override {
        if (index < 0 || index > 3) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        if (index == 3) return w;
        return x;
    }

    bool operator==(const SVector4<T>& other) const override
    {
        return std::abs(x - other.x) < RT_COMPARE_PRECISION &&
           std::abs(y - other.y) < RT_COMPARE_PRECISION &&
           std::abs(z - other.z) < RT_COMPARE_PRECISION &&
           std::abs(w - other.w) < RT_COMPARE_PRECISION;
    }
    bool operator!=(const SVector4<T>& other) const override
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

    T dot(const SVector4<T>& other) const override
    {
        return *this*other;
    }

    T cross2D(const SVector4<T>& other) const override {
        return 0;
    }

    SVector4<T> cross4D(const SVector4<T>& other) const override {
        return
        {
            y*other.z - z*other.y + w*other.x - x*other.w,
            z*other.x - x*other.z + w*other.y - y*other.w,
            x*other.y - y*other.x + w*other.z - z*other.w,
            x*other.w - w*other.x + y*other.z - z*other.y,
        };
    }

    SVector4<T> cross3D(const SVector4<T>& other) const override {
        return
        {
            y*other.z - z*other.y,
            z*other.x - x*other.z,
            x*other.y - y*other.x,
            0,
        };
    }

    SVector4<T> normalize() const override
    {
        T length = std::sqrt(x*x + y*y + z*z + w*w);
        if (length == static_cast<T>(0)) {
            return {0, 0, 0, 0};
        }
        SVector4<T> ret = {x / length, y / length, z/ length, w/length};
        return ret;
    }

    void normalizeSelf() override
    {
        T length = std::sqrt(x*x + y*y + z*z + w*w);
        if (length == static_cast<T>(0)) {
            x = y = z = w = 0;
        }
        x = x / length;
        y = y / length;
        z = z / length;
        w = w / length;
    }
    T length() const override
    {
        return std::sqrt(x*x + y*y + z*z + w*w);
    }
    T lengthSquared() const override
    {
        return x*x + y*y + z*z + w*w;
    }
    bool isZero() const override
    {
        return this->lengthSquared() < RT_COMPARE_PRECISION;
    }
    bool isNormalized() const override
    {
        return std::abs(this->lengthSquared() - 1.0f) < RT_COMPARE_PRECISION;
    }
    T distance(const SVector4<T>& other) const override
    {
        return std::sqrt(distanceSquared(other));
    }
    T distanceSquared(const SVector4<T>& other) const override
    {
        T dx = x - other.x;
        T dy = y - other.y;
        T dz = z - other.z;
        T dw = w - other.w;
        return dx * dx + dy * dy + dz * dz + dw * dw;
    }
    SVector4<T> lerp(const SVector4<T>& other, T t) const override
    {
        return SVector4<T>(
            x + (other.x - x) * t,
            y + (other.y - y) * t,
            z + (other.z - z) * t,
            w + (other.w - w) * t
        );
    }
    SVector4<T> reflect(const SVector4<T>& normal) const override
    {
        T dotProduct = this->dot(normal);
        return SVector4<T>(
            x - 2 * dotProduct * normal.x,
            y - 2 * dotProduct * normal.y,
            z - 2 * dotProduct * normal.z,
            w - 2 * dotProduct * normal.w
        );
    }
    SVector4<T> project(const SVector4<T>& normal) const override
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
public:
    T x;
    T y;
    T z;
    T w;
};
}

#endif
