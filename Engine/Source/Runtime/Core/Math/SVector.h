#ifndef CORE_MATH_SVector_H
#define CORE_MATH_SVector_H
#include "IVector.h"
#include <stdexcept> // For std::out_of_range
#include <vector>    // For std::vector
#include <initializer_list>
#include <algorithm>
#include <cmath>
#include <cassert>

namespace ReiToEngine
{
template <uint8_t DIM,typename T>
class SVector : public IVector<SVector<DIM, T>, DIM, T>
{
public:
    ~SVector()
    {
        std::cout << "默认析构"<< std::endl;
        delete[] data;
    }
    SVector(const std::initializer_list<T>& init) : data(new T[DIM]),
        x(data[0]),
        y(DIM >= 2 ? data[1] : data[0]),
        z(DIM >= 3 ? data[2] : data[0]),
        w(DIM >= 4 ? data[3] : data[0])
    {
        size_t list_size = init.size();
        list_size = list_size > DIM ? DIM : list_size;
        size_t i = 0;
        for(; i < list_size; ++ i)
        {
            data[i] = init.begin()[i];
        }
        for(; i < DIM; ++ i)
        {
            data[i] = T{0};
        }
    }
    SVector() : data(new T[DIM]),
        x(data[0]),
        y(DIM >= 2 ? data[1] : data[0]),
        z(DIM >= 3 ? data[2] : data[0]),
        w(DIM >= 4 ? data[3] : data[0])
    {
        std::cout << "默认初始化"<< std::endl;
        size_t i = 0;
        for(; i < DIM; ++ i)
        {
            data[i] = T{0};
        }
    }
    SVector(T val) : data(new T[DIM]),
        x(data[0]),
        y(DIM >= 2 ? data[1] : data[0]),
        z(DIM >= 3 ? data[2] : data[0]),
        w(DIM >= 4 ? data[3] : data[0])
    {
        size_t i = 0;
        for(; i < DIM; ++ i)
        {
            data[i] = T{val};
        }
    }
    SVector(const SVector<DIM, T>& other) : data(new T[DIM]),
        x(data[0]),
        y(DIM >= 2 ? data[1] : data[0]),
        z(DIM >= 3 ? data[2] : data[0]),
        w(DIM >= 4 ? data[3] : data[0])
    {
        std::cout << "拷贝构造"<< std::endl;

        size_t i = 0;
        for(; i < DIM; ++i)
        {
            data[i] = other.data[i];
        }
    }
    SVector(SVector<DIM, T>&& other) : data(other.data),
        x(data[0]),
        y(DIM >= 2 ? data[1] : data[0]),
        z(DIM >= 3 ? data[2] : data[0]),
        w(DIM >= 4 ? data[3] : data[0])
    {
        std::cout << "移动构造"<< std::endl;
        other.data = nullptr;
    }
    // SVector<DIM, T>ement functions from IVector (CRTP style, returning SVector<T, DIM>& and SVector<T, DIM>)
    SVector<DIM, T>& operator+=(const SVector<DIM, T>& other) override
    {
        size_t i = 0;
        for(; i < DIM; ++ i)
        {
            data[i] += other.data[i];
        }
        return *this;
    }
    SVector<DIM, T>& operator-=(const SVector<DIM, T>& other) override
    {
        size_t i = 0;
        for(; i < DIM; ++ i)
        {
            data[i] -= other.data[i];
        }
        return *this;
    }
    SVector<DIM, T>& operator*=(T scalar) override
    {
        size_t i = 0;
        for(; i < DIM; ++ i)
        {
            data[i] *= scalar;
        }
        return *this;
    }
    SVector<DIM, T>& operator/=(T scalar) override
    {
        size_t i = 0;
        for(; i < DIM; ++ i)
        {
            data[i] /= scalar;
        }
        return *this;
    }
    SVector<DIM, T> operator+(const SVector<DIM, T>& other) const override
    {
        SVector<DIM, T> result = *this;
        return result += other;
    }
    SVector<DIM, T> operator-(const SVector<DIM, T>& other) const override
    {
        SVector<DIM, T> result = *this;
        return result -= other;
    }
    SVector<DIM, T> operator*(T scalar) const override
    {
        SVector<DIM, T> result = *this;
        return result *= scalar;
    }
    SVector<DIM, T> operator/(T scalar) const override
    {
        SVector<DIM, T> result = *this;
        return result /= scalar;
    }
    T operator*(const SVector<DIM, T>& other) const override {
        T ret = T{};
        size_t i = 0;
        for(; i < DIM; ++ i)
        {
            ret += data[i] * other.data[i];
        }
        return ret;
    }

    T& operator[](int index) override {
        if (index < 0 || index > DIM - 1) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](int index) const override {
        if (index < 0 || index > DIM - 1) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    bool operator==(const SVector<DIM, T>& other) const override
    {
        size_t i = 0;
        for(; i < DIM; ++ i)
        {
            if (std::abs(data[i] - other.data[i]) < RT_COMPARE_PRECISION) return false;
        }
        return true;
    }
    bool operator!=(const SVector<DIM, T>& other) const override
    {
        return !(*this==other);
    }
    friend std::ostream& operator<<(std::ostream& os, const SVector<DIM, T>& vec)
    {

        os << "(";
        size_t i = 0;
        for(; i < DIM - 1; ++i)
        {
            os << vec.data[i] << ", ";
        }
        os <<vec.data[DIM - 1]<< ")" << std::endl;
        return os;
    }

    SVector<DIM, T>& operator=(const SVector<DIM, T>& other)
    {
        std::cout << "赋值运算符"<< std::endl;
        size_t i = 0;
        for(; i < DIM; ++ i)
        {
            data[i] = other.data[i];
        }
        return *this;
    }

    T dot(const SVector<DIM, T>& other) const override
    {
        return *this*other;
    }

    T cross2D(const SVector<DIM, T>& other) const override
    {
        assert(DIM >= 2 && "Call cross2D with a wrong DIMENTION.\n");
        return data[0]*other.data[1] - data[1]*other.data[0];
    }

    SVector<DIM, T> cross4D(const SVector<DIM, T>& other) const override {
        assert((DIM == 4 || DIM == 3) && "Call cross4D with a wrong DIMENTION.\n");
        if (DIM == 3) return cross3D(other);

        SVector<DIM, T> ret;
        ret[0] = data[1]*other.data[2] - data[2]*other.data[1] + data[3]* other.data[0] - data[0]*other.data[3];
        ret[1] = data[2]*other.data[0] - data[0]*other.data[2] + data[3]* other.data[1] - data[1]*other.data[3];
        ret[2] = data[0]*other.data[1] - data[1]*other.data[0] + data[3]* other.data[2] - data[2]*other.data[3];
        ret[3] = data[0]*other.data[3] - data[3]*other.data[0] + data[1]* other.data[2] - data[2]*other.data[1];
        return ret;
    }

    SVector<DIM, T> cross3D(const SVector<DIM, T>& other) const override {
        assert((DIM == 4 || DIM == 3) && "Call cross3D with a wrong DIMENTION.\n");

        SVector<DIM, T> ret;
        ret[0] = data[1]*other.data[2] - data[2]*other.data[1];
        ret[1] = data[2]*other.data[0] - data[0]*other.data[2];
        ret[2] = data[0]*other.data[1] - data[1]*other.data[0];
        return ret;
    }

    SVector<DIM, T> normalize() const override
    {
        T _length = length();
        if (_length == static_cast<T>(0)) {
            return SVector<DIM, T>();
        }
        SVector<DIM, T> ret = SVector();
        for(size_t i = 0; i < DIM; ++i)
        {
            ret.data[i] = data[i] / _length;
        }

        return ret;
    }

    void normalizeSelf() override
    {
        T _length = length();
        if (_length == static_cast<T>(0)) {
            for(size_t i = 0; i < DIM; ++i)
            {
                data[i] = 0;
            }
        }
        for(size_t i = 0; i < DIM; ++i)
        {
            data[i] = 0;
        }
    }
    T length() const override
    {
        return std::sqrt(lengthSquared());
    }
    T lengthSquared() const override
    {
        T ret = T{0};
        for(size_t i = 0; i < DIM; ++i)
        {
            ret += data[i]*data[i];
        }
        return ret;
    }
    bool isZero() const override
    {
        return this->lengthSquared() < RT_COMPARE_PRECISION;
    }
    bool isNormalized() const override
    {
        return std::abs(this->lengthSquared() - 1.0f) < RT_COMPARE_PRECISION;
    }
    T distance(const SVector<DIM, T>& other) const override
    {
        return std::sqrt(distanceSquared(other));
    }
    T distanceSquared(const SVector<DIM, T>& other) const override
    {
        T ret = T{0};
        for(size_t i = 0; i < DIM; ++i)
        {
            ret += (data[i] - other.data[i])*(data[i] - other.data[i]);
        }
        return ret;
    }
    SVector<DIM, T> lerp(const SVector<DIM, T>& other, T t) const override
    {
        SVector<DIM, T> ret;
        for(size_t i = 0; i < DIM; ++i)
        {
            ret.data[i] = data[i] + (other.data[i] - data[i]) * t;
        }
        return ret;
    }
    SVector<DIM, T> reflect(const SVector<DIM, T>& normal) const override
    {
        T dotProduct = this->dot(normal);

        SVector<DIM, T> ret;
        for(size_t i = 0; i < DIM; ++i)
        {
            ret.data[i] = data[i] - 2* dotProduct * normal.data[i];
        }
        return ret;
    }
    SVector<DIM, T> project(const SVector<DIM, T>& normal) const override
    {
        T dotProduct = this->dot(normal);
        T normalLengthSquared = normal.lengthSquared();
        T mult = dotProduct / normalLengthSquared;
        SVector<DIM, T> ret;
        for(size_t i = 0; i < DIM; ++i)
        {
            ret.data[i] = mult * normal.data[i];
        }
        return ret;
    }
public:
    T* data;
    T& x;
    T& y;
    T& z;
    T& w;
};
}

#endif
