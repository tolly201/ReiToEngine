#ifndef CORE_MATH_SVector_H
#define CORE_MATH_SVector_H
#include "IVector.h"
#include <stdexcept> // For std::out_of_range
#include <vector>    // For std::vector

namespace ReiToEngine
{
template <typename T> // SVector is now templated with T and DIM
class SVector3 : public IVector<SVector3<T>, 3, T> // CRTP inheritance: impl = SVector<T, DIM>
{
public:
    // Constructor
    SVector3() = default;

    // Implement virtual functions from IVector (CRTP style, returning SVector<T, DIM>& and SVector<T, DIM>)
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

    T& operator[](int index) override {
        if (index < 0 || index >= 2) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        return x;
    }

    const T& operator[](int index) const override {
        if (index < 0 || index >= 2) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        return x;
    }


public:
    T x;
    T y;
    T z;
};

template <typename T>
class SVector2 : public IVector<SVector2<T>, 2, T> // CRTP inheritance: impl = SVector<T, DIM>
{
public:
    ~SVector2() = default;
    T x;
    T y;
    // SVector2 specific code (if needed) can go here
};

typedef SVector2<int> Vec2i;
typedef SVector3<int> Vec3i;

}

#endif