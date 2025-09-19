#ifndef CORE_MATH_QUATERNION_H
#define CORE_MATH_QUATERNION_H
#include "SVector4.h"

namespace ReiToEngine
{
template <typename T>
class alignas((std::is_same_v<T, float>) ? 16 : alignof(T)) Quaternion : public SVector4<T>
{
public:
	using SVector4<T>::SVector4; // inherit constructors so {1,2,3,4} works
	// Conjugate (negate vector part, keep scalar part w)
	[[nodiscard]] Quaternion<T> conjugate() const noexcept
	{
		return Quaternion<T>(-this->x, -this->y, -this->z, this->w);
	}
	[[nodiscard]] T norm() const noexcept
	{
		return this->length();
	}
	[[nodiscard]] Quaternion<T> normalized() const noexcept
	{
		auto n = this->normalize();
		return Quaternion<T>(n.x, n.y, n.z, n.w);
	}
	// Inverse of quaternion: q^{-1} = conjugate(q) / |q|^2 (for unit quaternion equals conjugate)
	[[nodiscard]] Quaternion<T> inverse() const noexcept
	{
		T lsq = this->lengthSquared();
		if (lsq == static_cast<T>(0)) return Quaternion<T>();
		Quaternion<T> c = this->conjugate();
		return Quaternion<T>(c.x/lsq, c.y/lsq, c.z/lsq, c.w/lsq);
	}
	// Hamilton product
	[[nodiscard]] Quaternion<T> operator*(const Quaternion<T>& r) const noexcept
	{
		const Quaternion<T>& a = *this;
		return Quaternion<T>(
			a.w*r.x + a.x*r.w + a.y*r.z - a.z*r.y,
			a.w*r.y - a.x*r.z + a.y*r.w + a.z*r.x,
			a.w*r.z + a.x*r.y - a.y*r.x + a.z*r.w,
			a.w*r.w - a.x*r.x - a.y*r.y - a.z*r.z
		);
	}
};
}

#endif
