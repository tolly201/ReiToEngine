#ifndef CORE_MATH_SMatrix_H
#define CORE_MATH_SMatrix_H
#include "IMatrix.h"
#include <cstdint>
#include <cassert>
namespace ReiToEngine
{
template <uint8_t column, uint8_t row, typename T>
class RTENGINE_API SMatrix : public IMatrix<SMatrix<column, row, T>, column, row, T>
{
public:
    SMatrix()
    {
        data = new T[column * row];
        for (int i = 0; i < column * row; ++i) {
            data[i] = T{};
        }
    }

    SMatrix(const SMatrix& other)
    {
        data = new T[column * row];
        for (int i = 0; i < column * row; ++i) {
            data[i] = other.data[i];
        }
    }

    SMatrix(SMatrix&& other) noexcept
    {
        data = other.data;
        other.data = nullptr;
    }

    SMatrix(const std::initializer_list<T>& init)
    {
        int size = column * row;
        data = new T[column * row];
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
        data = new T[column * row];
        for (int i = 0; i < column * row; ++i) {
            data[i] = val;
        }
    }

    ~SMatrix()
    {
        delete[] data;
    }

    SMatrix<column, row, T>& operator+=(const SMatrix<column, row, T>& other) override {
        for (int i = 0; i < column * row; ++i) {
            data[i] += other.data[i];
        }
        return *this;
    }
    SMatrix<column, row, T>& operator-=(const SMatrix<column, row, T>& other) override {
        for (int i = 0; i < column * row; ++i) {
            data[i] -= other.data[i];
        }
        return *this;
    }
    SMatrix<column, row, T>& operator*=(T scalar) override {
        for (int i = 0; i < column * row; ++i) {
            data[i] *= scalar;
        }
        return *this;
    }

    SMatrix<column, row, T>& operator*=(const SMatrix<column, row, T>& other) override
    {
        // 检查 this 和 other 是否是同一个对象
        if (this == &other) {
            SMatrix<column, row, T> temp = *this;
            return *this *= temp;
        }

        T* tempData = new T[column * row];
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
        delete[] tempData;
        return *this; // 返回当前对象的引用
    }

    SMatrix<column, row, T> operator+(const SMatrix<column, row, T>& other) const override {
        SMatrix<column, row, T> result = *this;
        result += other;
        return result;
    }
    SMatrix<column, row, T> operator-(const SMatrix<column, row, T>& other) const override {
        SMatrix<column, row, T> result = *this;
        result -= other;
        return result;
    }

    SMatrix<column, row, T> operator*(T scalar) const override {
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

    Vector<column, T> operator[](int index) override
    {
        assert(index >= 0 && index < row && "Illegal row index when indexing matrix.\n");
        Vector<column, T> ret;
        for(size_t i = 0; i < column; ++i)
        {
            ret[i] = data[index * column + i];
        }
        return ret;
    }
    const Vector<column, T> operator[](int index) const override
    {
        assert(index >= 0 && index < row && "Illegal row index when indexing matrix.\n");
        Vector<column, T> ret;
        for(size_t i = 0; i < column; ++i)
        {
            ret[i] = data[index * column + i];
        }
        return ret;
    }

    SMatrix<column, row, T> transpose() const override
    {
        SMatrix<column, row, T> result;
        for (uint8_t i = 0; i < row; ++i) {
            for (uint8_t j = 0; j < column; ++j) {
                result.data[i * column + j] = data[j * row + i];
            }
        }
        return result;
    }
    void transposeSelf() const override
    {
        for (uint8_t i = 0; i < row; ++i) {
            for (uint8_t j = 0; j < column - row; ++j) {
                data[i * column + j] = data[j * row + i];
            }
        }
    }
    SMatrix<column, row, T> Inverse() const override
    {
        assert(column == row && "Inverse is only defined for square matrices.\n");

        SMatrix<column, row, T> augmented_l; // 创建增广矩阵
        SMatrix<column, row, T> augmented_r; // 创建增广矩阵
        // for (uint8_t i = 0; i < row; ++i)
        // {
        //     for (uint8_t j = 0; j < column; ++j)
        //     {
        //         augmented_l[i][j] = data[i * column + j];      // 左侧为原矩阵
        //         augmented_r[i][j] = (i == j) ? 1 : 0; // 右侧为单位矩阵
        //     }
        // }

        // // 高斯-约旦消元
        // for (uint8_t i = 0; i < row; ++i)
        // {
        //     // 找到当前列中绝对值最大的元素所在的行
        //     uint8_t maxRow = i;
        //     for (uint8_t k = i + 1; k < row; ++k)
        //     {
        //         if (std::abs(augmented[k][i]) > std::abs(augmented[maxRow][i]))
        //         {
        //             maxRow = k;
        //         }
        //     }

        //     // 如果当前列的最大元素为 0，则矩阵不可逆
        //     assert(std::abs(augmented[maxRow][i]) >= RT_COMPARE_PRECISION && "Matrix is not invertible. Max Variable equals zero.");

        //     // 将最大元素所在行交换到当前行
        //     if (maxRow != i)
        //     {
        //         for (uint8_t j = 0; j < 2 * column; ++j)
        //         {
        //             std::swap(augmented[i][j], augmented[maxRow][j]);
        //         }
        //     }

        //     // 将当前行的主元归一化
        //     T pivot = augmented[i][i];
        //     for (uint8_t j = 0; j < 2 * column; ++j)
        //     {
        //         augmented[i][j] /= pivot;
        //     }

        //     // 消去其他行中当前列的元素
        //     for (uint8_t k = 0; k < row; ++k)
        //     {
        //         if (k != i)
        //         {
        //             T factor = augmented[k][i];
        //             for (uint8_t j = 0; j < 2 * column; ++j)
        //             {
        //                 augmented[k][j] -= factor * augmented[i][j];
        //             }
        //         }
        //     }
        // }

        // // 提取逆矩阵
        SMatrix<column, row, T> inverse;
        // for (uint8_t i = 0; i < row; ++i)
        // {
        //     for (uint8_t j = 0; j < column; ++j)
        //     {
        //         inverse[i][j] = augmented[i][j + column];
        //     }
        // }

        return inverse;
    }
    void Invert() const override
    {
        SMatrix<column, row, T> inv = this->Inverse();
        for(size_t i = 0; i < column * row; ++i)
        {
            data[i] = inv.data[i];
        }
    }
    T Determinant() const override
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
    SMatrix<column, row, T> Multiply(const SMatrix<column, row, T>& other) const override
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
    SMatrix<column, row, T> operator*(const SMatrix<column, row, T>& other) const override
    {
        return this->Multiply(other);
    }
    uint8_t Rank() const
    {
        SMatrix<column, row, T> temp = *this; // 创建一个临时副本
        uint8_t rank = 0;
        uint8_t currentRow = 0;

        for (uint8_t col = 0; col < column; ++col) {
            // 找到当前列中绝对值最大的元素所在的行
            uint8_t maxRow = currentRow;
            for (uint8_t i = currentRow + 1; i < row; ++i) {
                if (std::abs(temp[i][col]) > std::abs(temp[maxRow][col])) {
                    maxRow = i;
                }
            }

            // 如果当前列的最大元素为 0，则跳过该列
            if (std::abs(temp[maxRow][col]) < RT_COMPARE_PRECISION) { // 使用一个小的阈值判断是否为 0
                continue;
            }

            // 将最大元素所在行交换到当前行
            if (maxRow != currentRow) {
                for (uint8_t j = 0; j < column; ++j) {
                    std::swap(temp[currentRow][j], temp[maxRow][j]);
                }
            }

            // 将当前行的主元归一化
            T pivot = temp[currentRow][col];
            for (uint8_t j = 0; j < column; ++j) {
                temp[currentRow][j] /= pivot;
            }

            // 消去其他行中当前列的元素
            for (uint8_t i = 0; i < row; ++i) {
                if (i != currentRow) {
                    T factor = temp[i][col];
                    for (uint8_t j = 0; j < column; ++j) {
                        temp[i][j] -= factor * temp[currentRow][j];
                    }
                }
            }

            rank++;
            currentRow++;
            if (currentRow >= row) break;
        }

        return rank;
    }
    Vector<column, T> Transform(const Vector<column, T>& vector) const override {}
    Vector<column, T> operator*(const Vector<column, T>& vector) const override {}
    SMatrix<column, row, T> GetRow(uint8_t rowIndex) const override
    {

    }
    SMatrix<column, row, T> GetColumn(uint8_t columnIndex) const override
    {}
    bool IsIdentity() const override
    {
        for (uint8_t i = 0; i < row; ++i) {
            for (uint8_t j = 0; j < column; ++j) {
                if (i == j && data[i * column + j] != 1) return false;
                if (i != j && data[i * column + j] != 0) return false;
            }
        }
        return true;
    }
    bool IsZero() const override
    {
        for (int i = 0; i < column * row; ++i)
        {
            if (data[i] != 0) return false;
        }
        return true;
    }
    bool IsOrthogonal() const override
    {
        SMatrix<column, row, T> transposed = this->transpose();
        SMatrix<column, row, T> product = this->Multiply(transposed);
        return product.IsIdentity();
    }
    bool IsSymmetric() const override
    {
        SMatrix<column, row, T> transposed = this->transpose();
        return *this == transposed;
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
    T* data;
};
}

#endif
