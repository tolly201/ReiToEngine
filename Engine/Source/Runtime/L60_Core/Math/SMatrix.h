#ifndef CORE_MATH_SMatrix_H
#define CORE_MATH_SMatrix_H
#include <cstdint>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <utility>
#include <new>
#include "L20_Platform/L31_SingletonFactory/SingletonFactory.h"
#include "VectorInclude.h" // For Vector<DIM, T> alias used in methods
namespace ReiToEngine
{
template <uint8_t column, uint8_t row, typename T>
class RTENGINE_API SMatrix
{
public:
    static constexpr uint8_t Columns = column;
    static constexpr uint8_t Rows = row;
    // Custom memory management
    static void* operator new(std::size_t sz)
    {
        return GetMemoryManager().Allocate(sz, static_cast<u8>(alignof(SMatrix)), RT_MEMORY_TAG::MATH);
    }
    static void operator delete(void* p, std::size_t sz) noexcept
    {
        if (p) GetMemoryManager().Free(p, sz, RT_MEMORY_TAG::MATH);
    }
#if __cpp_aligned_new
    static void* operator new(std::size_t sz, std::align_val_t al)
    {
        std::size_t a = static_cast<std::size_t>(al);
        u8 align_u8 = static_cast<u8>(a > 255 ? 255 : a);
        return GetMemoryManager().Allocate(sz, align_u8, RT_MEMORY_TAG::MATH);
    }
    static void operator delete(void* p, std::size_t sz, std::align_val_t) noexcept
    {
        if (p) GetMemoryManager().Free(p, sz, RT_MEMORY_TAG::MATH);
    }
#endif
    static void* operator new[](std::size_t sz)
    {
        return GetMemoryManager().Allocate(sz, static_cast<u8>(alignof(SMatrix)), RT_MEMORY_TAG::MATH);
    }
    static void operator delete[](void* p, std::size_t sz) noexcept
    {
        if (p) GetMemoryManager().Free(p, sz, RT_MEMORY_TAG::MATH);
    }
    T data[column * row]; // 改为固定大小数组而不是动态分配

    SMatrix()
    {
        for (int i = 0; i < column * row; ++i) {
            data[i] = T{};
        }
    }

    SMatrix(const SMatrix& other)
    {
        for (int i = 0; i < column * row; ++i) {
            data[i] = other.data[i];
        }
    }

    SMatrix(SMatrix&& other) noexcept
    {
        for (int i = 0; i < column * row; ++i) {
            data[i] = other.data[i];
        }
    }

    SMatrix(const std::initializer_list<T>& init)
    {
        int size = column * row;
        int list_size = init.size();
        list_size = list_size > size ? size : list_size;
        int i = 0;
        for (; i < list_size; ++i) {
            data[i] = init.begin()[i];
        }
        // 如果初始化列表的元素少于矩阵大小，剩余元素设置为 0
        for (; i < size; ++i) {
            data[i] = T{};
        }
    }

    SMatrix(T val)
    {
        for (int i = 0; i < column * row; ++i) {
            data[i] = val;
        }
    }

    ~SMatrix() = default; // 不再需要析构函数

    SMatrix<column, row, T>& operator+=(const SMatrix<column, row, T>& other) {
        for (int i = 0; i < column * row; ++i) {
            data[i] += other.data[i];
        }
        return *this;
    }
    SMatrix<column, row, T>& operator-=(const SMatrix<column, row, T>& other) {
        for (int i = 0; i < column * row; ++i) {
            data[i] -= other.data[i];
        }
        return *this;
    }
    SMatrix<column, row, T>& operator*=(T scalar) {
        for (int i = 0; i < column * row; ++i) {
            data[i] *= scalar;
        }
        return *this;
    }

    SMatrix<column, row, T>& operator*=(const SMatrix<column, row, T>& other)
    {
        // 检查 this 和 other 是否是同一个对象
        if (this == &other) {
            SMatrix<column, row, T> temp = *this;
            return *this *= temp;
        }

        T tempData[column * row];
        for (size_t i = 0; i < column * row; ++i) {
            tempData[i] = data[i];
        }

        for (size_t i = 0; i < row; ++i) {
            for (size_t j = 0; j < column; ++j) {
                T sum = 0;
                for (size_t k = 0; k < column; ++k) {
                    sum += tempData[i * column + k] * other.data[k * row + j]; // 注意这里的索引
                }
                data[i * column + j] = sum; // 直接更新当前对象的数据
            }
        }
        return *this; // 返回当前对象的引用
    }

    [[nodiscard]] SMatrix<column, row, T> operator+(const SMatrix<column, row, T>& other) const {
        SMatrix<column, row, T> result = *this;
        result += other;
        return result;
    }
    [[nodiscard]] SMatrix<column, row, T> operator-(const SMatrix<column, row, T>& other) const {
        SMatrix<column, row, T> result = *this;
        result -= other;
        return result;
    }

    [[nodiscard]] SMatrix<column, row, T> operator*(T scalar) const {
        SMatrix<column, row, T> result = *this;
        result *= scalar;
        return result;
    }

    bool operator ==(const SMatrix<column, row, T>& other) const
    {
        size_t size = column * row;
        for(size_t i = 0; i < size; ++i)
        {
            if(other.data[i]!=data[i]) return false;
        }
        return true;
    }
    bool operator!=(const SMatrix<column, row, T>& other) const
    {
        return !(this->operator==(other));
    }
    SMatrix<column, row, T>& operator=(const SMatrix<column, row, T>& other)
    {
        if (this != &other) {
            for (size_t i=0; i < column*row;++i) data[i] = other.data[i];
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const SMatrix<column, row, T>& vec)
    {
        for(uint8_t r = 0; r < row; ++r)
        {
            os << "(";
            for(uint8_t c = 0; c < column; ++c) {
                os << vec.data[r * column + c];
                if (c + 1 < column) os << ", ";
            }
            os << ")" << std::endl;
        }
        return os;
    }


    Vector<column, T> operator[](int index)
    {
        assert(index >= 0 && index < row && "Illegal row index when indexing matrix.\n");
        Vector<column, T> ret;
        for(size_t i = 0; i < column; ++i)
        {
            ret[i] = data[index * column + i];
        }
        return ret;
    }
    const Vector<column, T> operator[](int index) const
    {
        assert(index >= 0 && index < row && "Illegal row index when indexing matrix.\n");
        Vector<column, T> ret;
        for(size_t i = 0; i < column; ++i)
        {
            ret[i] = data[index * column + i];
        }
        return ret;
    }

    [[nodiscard]] SMatrix<column, row, T> transpose() const
    {
        SMatrix<column, row, T> result;
        for (uint8_t i = 0; i < row; ++i) {
            for (uint8_t j = 0; j < column; ++j) {
                result.data[j * row + i] = data[i * column + j];
            }
        }
        return result;
    }
    void transposeSelf()
    {
        // Only valid for square matrices. For non-square, use transpose() to a new matrix.
        assert(column == row && "transposeSelf requires square matrix");
        for (uint8_t i = 0; i < row; ++i) {
            for (uint8_t j = i + 1; j < column; ++j) {
                std::swap(data[i * column + j], data[j * column + i]);
            }
        }
    }
    [[nodiscard]] SMatrix<column, row, T> Inverse() const
    {
        assert(column == row && "Inverse is only defined for square matrices.\n");

        constexpr uint8_t N = column;
        SMatrix<N, N, T> left = *this;   // 左侧为原矩阵
        SMatrix<N, N, T> right;          // 右侧初始化为单位矩阵
        for (uint8_t i = 0; i < N; ++i) {
            for (uint8_t j = 0; j < N; ++j) {
                right.data[i * N + j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
            }
        }

        // 高斯-约旦消元，带部分选主元
        for (uint8_t col = 0, rowIdx = 0; col < N && rowIdx < N; ++col) {
            // 选择当前列绝对值最大的行
            uint8_t pivotRow = rowIdx;
            for (uint8_t r = rowIdx + 1; r < N; ++r) {
                if (std::abs(left.data[r * N + col]) > std::abs(left.data[pivotRow * N + col])) {
                    pivotRow = r;
                }
            }

            // 若主元素太小，视为奇异，返回零矩阵
            if (std::abs(left.data[pivotRow * N + col]) < RT_COMPARE_PRECISION) {
                return SMatrix<N, N, T>(static_cast<T>(0));
            }

            // 交换当前工作行和主元行（左右两侧同时交换）
            if (pivotRow != rowIdx) {
                for (uint8_t j = 0; j < N; ++j) {
                    std::swap(left.data[rowIdx * N + j],  left.data[pivotRow * N + j]);
                    std::swap(right.data[rowIdx * N + j], right.data[pivotRow * N + j]);
                }
            }

            // 归一化主元行
            T pivot = left.data[rowIdx * N + col];
            for (uint8_t j = 0; j < N; ++j) {
                left.data[rowIdx * N + j]  /= pivot;
                right.data[rowIdx * N + j] /= pivot;
            }

            // 消去其它行该列
            for (uint8_t r = 0; r < N; ++r) {
                if (r == rowIdx) continue;
                T factor = left.data[r * N + col];
                if (std::abs(factor) < RT_COMPARE_PRECISION) continue;
                for (uint8_t j = 0; j < N; ++j) {
                    left.data[r * N + j]  -= factor * left.data[rowIdx * N + j];
                    right.data[r * N + j] -= factor * right.data[rowIdx * N + j];
                }
            }

            ++rowIdx;
        }

        // 此时 left 应为单位矩阵，right 为逆矩阵
        return right;
    }
    void Invert()
    {
        SMatrix<column, row, T> inv = this->Inverse();
        for(size_t i = 0; i < column * row; ++i)
        {
            data[i] = inv.data[i];
        }
    }
    [[nodiscard]] T Determinant() const
    {
        assert(column == row && "Determinant is only defined for square matrices.\n");

        if constexpr (column == 2)
        {
            return data[0] * data[3] - data[1] * data[2];
        } else if constexpr (column == 3)
        {
            return data[0] * (data[4] * data[8] - data[5] * data[7])
                 - data[1] * (data[3] * data[8] - data[5] * data[6])
                 + data[2] * (data[3] * data[7] - data[4] * data[6]);
        }
        return T{0};
        // else
        // {
        //     T det = 0;
        //     for (uint8_t c = 0; c < column; ++c) {
        //         SMatrix<column - 1, row - 1, T> submatrix;
        //         for (uint8_t i = 1; i < row; ++i) {
        //             for (uint8_t j = 0; j < column; ++j) {
        //                 if (j < c) {
        //                     submatrix.data[(i - 1) * (column - 1) + j] = data[i * column + j];
        //                 } else if (j > c) {
        //                     submatrix.data[(i - 1) * (column - 1) + j - 1] = data[i * column + j];
        //                 }
        //             }
        //         }
        //         det += (c % 2 == 0 ? 1 : -1) * data[c] * submatrix.Determinant();
        //     }
        //     return det;
        // }
    }
    // static SMatrix<column, row, T> Identity() override {}
    // static SMatrix<column, row, T> Zero() override {}
    [[nodiscard]] SMatrix<column, row, T> Multiply(const SMatrix<column, row, T>& other) const
    {
        SMatrix<column, row, T> result;
        for (uint8_t i = 0; i < row; ++i) {
            for (uint8_t j = 0; j < column; ++j) {
                T sum = 0;
                for (uint8_t k = 0; k < column; ++k) {
                    sum += data[i * column + k] * other.data[k * column + j];
                }
                result.data[i * column + j] = sum;
            }
        }
        return result;
    }
    [[nodiscard]] SMatrix<column, row, T> operator*(const SMatrix<column, row, T>& other) const
    {
        return this->Multiply(other);
    }
    [[nodiscard]] uint8_t Rank() const
    {
        SMatrix<column, row, T> temp = *this; // 创建一个临时副本
        uint8_t rank = 0;
        uint8_t currentRow = 0;

        for (uint8_t col = 0; col < column && currentRow < row; ++col) {
            // 找到当前列中绝对值最大的元素所在的行
            uint8_t maxRow = currentRow;
            for (uint8_t i = currentRow + 1; i < row; ++i) {
                if (std::abs(temp.data[i * column + col]) > std::abs(temp.data[maxRow * column + col])) {
                    maxRow = i;
                }
            }

            // 如果当前列的最大元素为 0，则跳过该列
            if (std::abs(temp.data[maxRow * column + col]) < RT_COMPARE_PRECISION) {
                continue;
            }

            // 将最大元素所在行交换到当前行
            if (maxRow != currentRow) {
                for (uint8_t j = 0; j < column; ++j) {
                    std::swap(temp.data[currentRow * column + j], temp.data[maxRow * column + j]);
                }
            }

            // 将当前行的主元归一化
            T pivot = temp.data[currentRow * column + col];
            for (uint8_t j = 0; j < column; ++j) {
                temp.data[currentRow * column + j] /= pivot;
            }

            // 消去其他行中当前列的元素
            for (uint8_t i = 0; i < row; ++i) {
                if (i == currentRow) continue;
                T factor = temp.data[i * column + col];
                if (std::abs(factor) < RT_COMPARE_PRECISION) continue;
                for (uint8_t j = 0; j < column; ++j) {
                    temp.data[i * column + j] -= factor * temp.data[currentRow * column + j];
                }
            }

            rank++;
            currentRow++;
        }

        return rank;
    }
    [[nodiscard]] Vector<column, T> Transform(const Vector<column, T>& vector) const
    {
        Vector<column, T> result;
        for (size_t i = 0; i < row; ++i) {
            T sum = T{0};
            for (size_t j = 0; j < column; ++j) {
                sum += data[i * column + j] * vector[j];
            }
            result[i] = sum;
        }
        return result;
    }

    Vector<column, T> operator*(const Vector<column, T>& vector) const
    {
        return Transform(vector);
    }
    [[nodiscard]] T Trace() const
    {
        static_assert(column == row, "Trace is only defined for square matrices.");
        T sum = T{0};
        for (uint8_t i = 0; i < row; ++i) sum += data[i * column + i];
        return sum;
    }
    [[nodiscard]] Vector<column, T> GetRow(uint8_t rowIndex) const
    {
        assert(rowIndex < row && "Illegal row index when indexing matrix.\n");
        Vector<column, T> ret;
        for (size_t i = 0; i < column; ++i) ret[i] = data[rowIndex * column + i];
        return ret;
    }
    [[nodiscard]] Vector<row, T> GetColumn(uint8_t columnIndex) const
    {
        assert(columnIndex < column && "Illegal column index when indexing matrix.\n");
        Vector<row, T> ret;
        for (size_t i = 0; i < row; ++i) ret[i] = data[i * column + columnIndex];
        return ret;
    }
    void SetRow(uint8_t rowIndex, const Vector<column, T>& v)
    {
        assert(rowIndex < row && "Illegal row index when indexing matrix.\n");
        for (size_t i = 0; i < column; ++i) data[rowIndex * column + i] = v[i];
    }
    void SetColumn(uint8_t columnIndex, const Vector<row, T>& v)
    {
        assert(columnIndex < column && "Illegal column index when indexing matrix.\n");
        for (size_t i = 0; i < row; ++i) data[i * column + columnIndex] = v[i];
    }
    [[nodiscard]] bool IsIdentity() const
    {
        for (uint8_t i = 0; i < row; ++i) {
            for (uint8_t j = 0; j < column; ++j) {
                if (i == j && data[i * column + j] != static_cast<T>(1)) return false;
                if (i != j && data[i * column + j] != static_cast<T>(0)) return false;
            }
        }
        return true;
    }
    [[nodiscard]] bool IsZero() const
    {
        for (int i = 0; i < column * row; ++i)
        {
            if (data[i] != static_cast<T>(0)) return false;
        }
        return true;
    }
    [[nodiscard]] bool IsOrthogonal() const
    {
        SMatrix<column, row, T> transposed = this->transpose();
        SMatrix<column, row, T> product = this->Multiply(transposed);
        return product.IsIdentity();
    }
    [[nodiscard]] bool IsSymmetric() const
    {
        SMatrix<column, row, T> transposed = this->transpose();
        return *this == transposed;
    }
    // Static constructors
    static SMatrix<column, row, T> Zero()
    {
        return SMatrix<column, row, T>(static_cast<T>(0));
    }
    static SMatrix<column, row, T> Identity()
    {
        static_assert(column == row, "Identity is only defined for square matrices.");
        SMatrix<column, row, T> m(static_cast<T>(0));
        for (uint8_t i = 0; i < row; ++i)
        {
            m.data[i * column + i] = static_cast<T>(1);
        }
        return m;
    }
    // LU 分解 (Doolittle 算法)
    std::pair<SMatrix<column, row, T>, SMatrix<column, row, T>> LUDecomposition() const
    {
        static_assert(column == row, "LU decomposition is only defined for square matrices.");

        SMatrix<column, row, T> lower; // 下三角矩阵 L (对角线元素为 1)
        SMatrix<column, row, T> upper = *this; // 上三角矩阵 U (初始为原矩阵)

        for (uint8_t i = 0; i < column; ++i) {
            // 计算 L 矩阵的第 i 列
            for (uint8_t j = i; j < row; ++j) {
                T sum = 0;
                for (uint8_t k = 0; k < i; ++k) {
                    sum += lower.data[j * column + k] * upper.data[k * column + i];
                }
                lower.data[j * column + i] = upper.data[j * column + i] - sum;
            }

            // 计算 U 矩阵的第 i+1 行 (从 i+1 开始，因为 U 的对角线元素在上面已经计算过了)
            for (uint8_t j = i + 1; j < column; ++j) {
                T sum = 0;
                for (uint8_t k = 0; k < i; ++k) {
                    sum += lower.data[i * column + k] * upper.data[k * column + j];
                }
                if (lower.data[i * column + i] == 0) {
                    //throw std::runtime_error("LU decomposition failed: division by zero.");
                    // 或者你可以选择交换行，但那会更复杂，需要一个 P 矩阵 (置换矩阵)
                    return std::make_pair(SMatrix<column,row,T>(0), SMatrix<column,row,T>(0));
                }
                upper.data[i * column + j] = (upper.data[i * column + j] - sum) / lower.data[i * column + i];
            }
        }
        //设置L矩阵的对角线为1
        for (size_t i = 0; i < column; ++i)
        {
            lower.data[i * column + i] = 1;
        }

        return std::make_pair(lower, upper);
    }
private:
};
}

#endif
