#include "../Core/TestCore.h"
#include "L60_Core/Math/SMatrix.h"

using namespace ReiToEngine;

RT_TEST_ST("Math", SMatrix_inverse_2x2){
    // A = [4 7; 2 6], inv(A) = (1/10) * [6 -7; -2 4]
    SMatrix<2,2,double> A{4,7, 2,6};
    auto inv = A.Inverse();
    // A * inv should be identity (within tolerance)
    auto I = A * inv;
    RT_EXPECT_NEAR(I.data[0], 1.0, 1e-9);
    RT_EXPECT_NEAR(I.data[1], 0.0, 1e-9);
    RT_EXPECT_NEAR(I.data[2], 0.0, 1e-9);
    RT_EXPECT_NEAR(I.data[3], 1.0, 1e-9);
}

RT_TEST_ST("Math", SMatrix_inverse_3x3){
    // 选一个可逆 3x3
    SMatrix<3,3,double> A{ 3,0,2, 2,0,-2, 0,1,1 };
    auto inv = A.Inverse();
    auto I = A * inv;
    // 近似单位阵
    RT_EXPECT_NEAR(I.data[0], 1.0, 1e-9);
    RT_EXPECT_NEAR(I.data[1], 0.0, 1e-9);
    RT_EXPECT_NEAR(I.data[2], 0.0, 1e-9);
    RT_EXPECT_NEAR(I.data[3], 0.0, 1e-9);
    RT_EXPECT_NEAR(I.data[4], 1.0, 1e-9);
    RT_EXPECT_NEAR(I.data[5], 0.0, 1e-9);
    RT_EXPECT_NEAR(I.data[6], 0.0, 1e-9);
    RT_EXPECT_NEAR(I.data[7], 0.0, 1e-9);
    RT_EXPECT_NEAR(I.data[8], 1.0, 1e-9);
}

RT_TEST_ST("Math", SMatrix_inverse_singular_returns_zero){
    // 奇异矩阵（行线性相关）
    SMatrix<3,3,float> S{1,2,3, 2,4,6, 3,6,9};
    auto inv = S.Inverse();
    RT_EXPECT_TRUE(inv.IsZero());
}
