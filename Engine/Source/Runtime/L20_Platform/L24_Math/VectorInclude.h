#ifndef CORE_MATH_VECTOR_INCLUDE_H
#define CORE_MATH_VECTOR_INCLUDE_H
#include "SVector.h"
#include "SVector2.h"
#include "SVector3.h"
#include "SVector4.h"

namespace ReiToEngine
{
typedef SVector<2, int> Vec2i;
typedef SVector<3, int> Vec3i;
typedef SVector<4, int> Vec4i;

typedef SVector<2, double> Vec2d;
typedef SVector<3, double> Vec3d;
typedef SVector<4, double> Vec4d;

typedef SVector<2, float> Vec2f;
typedef SVector<3, float> Vec3f;
typedef SVector<4, float> Vec4f;

template<uint8_t DIM, typename T>
using Vector = SVector<DIM, T>;
}

#endif
