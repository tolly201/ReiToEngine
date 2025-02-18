#ifndef CORE_MATH_SVector_H
#define CORE_MATH_SVector_H
#include "IVector.h"
#include <stdexcept> // For std::out_of_range
#include <vector>    // For std::vector

namespace ReiToEngine
{
template <typename T, int DIM> // SVector is now templated with T and DIM
class SVector : public IVector<SVector<T, DIM>, DIM, T> // CRTP inheritance: impl = SVector<T, DIM>
{
public:
    // Constructor
    SVector() : data(DIM) {} // Initialize vector with DIM elements
    SVector(std::initializer_list<T> list) : data(list) {
        if (list.size() != DIM) {
            throw std::invalid_argument("Initializer list size does not match vector dimension.");
        }
    }

    // Implement virtual functions from IVector (CRTP style, returning SVector<T, DIM>& and SVector<T, DIM>)
    SVector<T, DIM>& operator+=(const SVector<T, DIM>& other) override
    {
        if (DIM != SVector<T, DIM>::Dimension || DIM != other.Dimension) {
            throw std::invalid_argument("Vector dimensions must match for addition.");
        }
        for (int i = 0; i < DIM; ++i) {
            data[i] += other.data[i];
        }
        return *this;
    }

    SVector<T, DIM>& operator-=(const SVector<T, DIM>& other) override
    {
        if (DIM != SVector<T, DIM>::Dimension || DIM != other.Dimension) {
            throw std::invalid_argument("Vector dimensions must match for subtraction.");
        }
        for (int i = 0; i < DIM; ++i) {
            data[i] -= other.data[i];
        }
        return *this;
    }

    SVector<T, DIM>& operator*=(T scalar) override
    {
        for (int i = 0; i < DIM; ++i) {
            data[i] *= scalar;
        }
        return *this;
    }

    SVector<T, DIM> operator+(const SVector<T, DIM>& other) const override
    {
        SVector<T, DIM> result = *this;
        return result += other;
    }

    SVector<T, DIM> operator-(const SVector<T, DIM>& other) const override
    {
        SVector<T, DIM> result = *this;
        return result -= other;
    }

    SVector<T, DIM> operator*(T scalar) const override
    {
        SVector<T, DIM> result = *this;
        return result *= scalar;
    }

    T& operator[](int index) override {
        if (index < 0 || index >= DIM) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](int index) const override {
        if (index < 0 || index >= DIM) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }


private:
    std::vector<T> data; // Use std::vector to store vector elements dynamically
};

template <typename T>
class SVector3 : public SVector<T, 3> // SVector3 inherits from SVector<T, 3>
{
public:
    // SVector3 specific code (if needed) can go here
    T x;
    T y;
    T z;
};

template <typename T>
class SVector2 : public SVector<T, 2> // SVector2 inherits from SVector<T, 2>
{
public:
    ~SVector2() = default;
    T x;
    T y;
    // SVector2 specific code (if needed) can go here
};

typedef SVector2<int> Vec2i;
typedef SVector3<int> Vec3i;
typedef SVector<float, 2> Vec2f;
typedef SVector<float, 3> Vec3f;
typedef SVector<double, 4> Vec4d; // Example of a 4D vector

}

#endif