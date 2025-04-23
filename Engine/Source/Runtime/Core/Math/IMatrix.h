#ifndef CORE_MATH_IMatrix_H
#define CORE_MATH_IMatrix_H
#include "Core/Macro/Macro.h"
#include <cstdint>
#include "VectorInclude.h"
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
    virtual impl& operator*=(const impl& other) = 0;

    virtual impl operator+(const impl& other) const = 0;
    virtual impl operator-(const impl& other) const = 0;
    virtual impl operator*(T scalar) const = 0;
    virtual bool operator==(const impl& other) const = 0;
    virtual bool operator!=(const impl& other) const = 0;
    virtual impl& operator=(const impl& other) = 0;

    virtual Vector<column, T> operator[](int index) = 0;
    virtual const Vector<column, T> operator[](int index) const = 0;

    virtual impl transpose() const = 0;
    virtual void transposeSelf() = 0;
    virtual impl Inverse() const = 0;
    virtual void Invert() = 0;
    virtual T Determinant() const = 0;
    // virtual static impl Identity() = 0;
    // virtual static impl Zero() = 0;
    virtual impl Multiply(const impl& other) const = 0;
    virtual impl operator*(const impl& other) const = 0;
    virtual uint8_t Rank() const = 0;
    virtual Vector<column, T> Transform(const Vector<column, T>& vector) const = 0;
    virtual Vector<column, T> operator*(const Vector<column, T>& vector) const = 0;
    virtual impl GetRow(uint8_t rowIndex) const = 0;
    virtual impl GetColumn(uint8_t columnIndex) const = 0;
    virtual bool IsIdentity() const = 0;
    virtual bool IsZero() const = 0;
    virtual bool IsOrthogonal() const = 0;
    virtual bool IsSymmetric() const = 0;
};
template <class impl, uint8_t column, uint8_t row, typename T>
IMatrix<impl, column,row, T>::~IMatrix() = default;
}

#endif
