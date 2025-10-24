#include "../Core/TestCore.h"
#include "L20_Platform/L24_Math/Include.h"
using namespace ReiToEngine;

RT_TEST_ST("Math", SMatrix_static_zero_identity){
    auto Z = SMatrix<3,3,float>::Zero();
    RT_EXPECT_TRUE(Z.IsZero());

    auto I = SMatrix<3,3,int>::Identity();
    RT_EXPECT_TRUE(I.IsIdentity());

    // Zero should be absorbing for addition with identity when multiplied by zero
    auto A = SMatrix<3,3,float>{1,2,3,4,5,6,7,8,9};
    auto R = A + SMatrix<3,3,float>::Zero();
    RT_EXPECT_TRUE(R == A);
}
