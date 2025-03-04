#ifndef CORE_MATH_IVECTOR_H
#define CORE_MATH_IVECTOR_H
#include "Core/HAL/Macro/Macro.h"
#include <cstdint>
#include <iostream>
namespace ReiToEngine
{
template <class impl, uint8_t DIM, typename T>
class RTENGINE_API IVector
{
public:
    static constexpr uint8_t Dimension = DIM;
    virtual ~IVector() = 0;

    virtual impl& operator+=(const impl& other) = 0;
    virtual impl& operator-=(const impl& other) = 0;
    virtual impl& operator*=(T scalar) = 0;
    virtual impl& operator/=(T scalar) = 0;

    virtual impl operator+(const impl& other) const = 0;
    virtual impl operator-(const impl& other) const = 0;
    virtual impl operator*(T scalar) const = 0;
    virtual impl operator/(T scalar) const = 0;
    virtual T operator*(const impl& other) const = 0;

    virtual T& operator[](int index) = 0;
    virtual const T& operator[](int index) const = 0;
    virtual bool operator==(const impl& other) const = 0;
    virtual bool operator!=(const impl& other) const = 0;
    virtual impl& operator=(const impl& other) = 0;

    virtual T dot(const impl& other) const = 0;
    virtual T cross2D(const impl& other) const = 0;
    virtual impl cross3D(const impl& other) const = 0;
    virtual impl cross4D(const impl& other) const = 0;
    virtual impl normalize() const = 0;
    virtual void normalizeSelf() = 0;
    virtual T length() const = 0;
    virtual T lengthSquared() const = 0;
    virtual bool isZero() const = 0;
    virtual bool isNormalized() const = 0;
    virtual T distance(const impl& other) const = 0;
    virtual T distanceSquared(const impl& other) const = 0;
    virtual impl lerp(const impl& other, T t) const = 0;
    virtual impl reflect(const impl& normal) const = 0;
    virtual impl project(const impl& normal) const = 0;
};
template <class impl, uint8_t DIM, typename T>
IVector<impl, DIM, T>::~IVector() = default;
}

#endif
