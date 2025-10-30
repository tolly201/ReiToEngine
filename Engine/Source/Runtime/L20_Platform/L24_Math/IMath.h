#ifndef CORE_MATH_IMATH_H
#define CORE_MATH_IMATH_H

#include <type_traits>
#include <cmath>
#include "VectorInclude.h"
#include "SMatrix.h"
namespace ReiToEngine
{

// High-precision pi constant for arbitrary floating point type
template <typename T>
constexpr T pi_v = static_cast<T>(3.141592653589793238462643383279502884L);

// Degrees <-> Radians conversion (enabled only for floating point types)
template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
constexpr T deg2rad(T deg) noexcept {
	return deg * (pi_v<T> / static_cast<T>(180));
}

template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
constexpr T rad2deg(T rad) noexcept {
	return rad * (static_cast<T>(180) / pi_v<T>);
}

// Scalar trig wrappers (radian versions). Prefer using std::sin/cos/tan directly when suitable.
template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
inline T sin_rad(T r) noexcept { return std::sin(r); }
template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
inline T cos_rad(T r) noexcept { return std::cos(r); }
template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
inline T tan_rad(T r) noexcept { return std::tan(r); }

// Scalar trig wrappers (degree versions)
template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
inline T sin_deg(T deg) noexcept { return std::sin(deg2rad(deg)); }
template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
inline T cos_deg(T deg) noexcept { return std::cos(deg2rad(deg)); }
template <typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0>
inline T tan_deg(T deg) noexcept { return std::tan(deg2rad(deg)); }

typedef SMatrix<2,2,double> Matrix2x2d;
typedef SMatrix<3,3,double> Matrix3x3d;
typedef SMatrix<4,4,double> Matrix4x4d;

typedef SMatrix<2,2,float> Matrix2x2f;
typedef SMatrix<3,3,float> Matrix3x3f;
typedef SMatrix<4,4,float> Matrix4x4f;

}
#endif
