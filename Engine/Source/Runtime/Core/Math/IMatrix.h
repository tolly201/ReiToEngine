#ifndef CORE_MATH_IMatrix_H
#define CORE_MATH_IMatrix_H
#include "Core/HAL/Macro/Macro.h"
#include <cstdint>
namespace ReiToEngine
{
template <class impl, uint8_t column, uint8_t row, typename T>
class RTENGINE_API IMatrix
{
public:
    virtual ~IMatrix() = 0;
    virtual impl& operator+=(const impl& other) = 0;
    virtual impl& operator-=(const impl& other) = 0;
    virtual impl& operator*=(T scalar) = 0;

    virtual impl operator+(const impl& other) const = 0;
    virtual impl operator-(const impl& other) const = 0;
    virtual impl operator*(T scalar) const = 0;

    virtual T& operator[](int index) = 0;
    virtual const T& operator[](int index) const = 0;

    virtual impl transpose() const = 0;
    virtual void transposeSelf() const = 0;
    virtual impl Inverse() const = 0;
    virtual void Invert() const = 0;
    virtual T Determinant() const = 0;
    virtual static impl Identity() = 0;
    virtual static impl Zero() = 0;
    virtual impl Multiply(const impl& other) const = 0;
    virtual impl operator*(const impl& other) const = 0;
    // virtual impl Transform(const impl& vector) const = 0;
    // virtual impl operator*(const impl& vector) const = 0;
    virtual impl GetRow(uint8_t rowIndex) const = 0;
    virtual impl GetColumn(uint8_t columnIndex) const = 0;
    virtual bool IsIdentity() const = 0;
    virtual bool IsZero() const = 0;
    virtual bool IsOrthogonal() const = 0;
    virtual bool IsSymmetric() const = 0;
};
}

#endif
