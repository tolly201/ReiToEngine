#ifndef CORE_MATH_IVECTOR_H
#define CORE_MATH_IVECTOR_H
#include "Core/HAL/Macro/Macro.h"

namespace ReiToEngine
{
template <class impl, uint8_t DIM, typename T>
class RTENGINE_API IVector
{
public:
    static constexpr uint8_t Dimension = DIM;
    virtual ~IVector() = 0;

    // Arithmetic operators (templated for impl and DIM)
    virtual impl& operator+=(const impl& other) = 0;
    virtual impl& operator-=(const impl& other) = 0;
    virtual impl& operator*=(T scalar) = 0;

    // Binary operators (templated for impl and DIM)
    virtual impl operator+(const impl& other) const = 0;
    virtual impl operator-(const impl& other) const = 0;
    virtual impl operator*(T scalar) const = 0;

    // Accessor for elements (optional, but useful)
    virtual T& operator[](int index) = 0;
    virtual const T& operator[](int index) const = 0;
};
template <class impl, uint8_t DIM, typename T>
IVector<impl, DIM, T>::~IVector() = default;
}

#endif
