#ifndef CORE_MATH_IMATH_H
#define CORE_MATH_IMATH_H

namespace ReiToEngine
{
template <typename T>
class IVector3
{
public:
    virtual ~IVector3() = 0;
};

template <typename T>
IVector3<T>::~IVector3() = default;  // 新增的析构函数实现

template <typename T>
class SVector3 : IVector3<T>
{
public:
    T x;
    T y;
    T z;
};

template <typename T>
class IVector2
{
public:
    virtual ~IVector2() = 0;
};

template <typename T>
IVector2<T>::~IVector2() = default;  // 新增的析构函数实现

template <typename T>
class SVector2 : IVector2<T>
{
public:
    ~SVector2() = default;

    T x;
    T y;
};

typedef SVector2<int> Vec2i;
typedef SVector3<int> Vec3i;

}

#endif