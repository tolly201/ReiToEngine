#include "MathTest.h"
#include "L60_Core/Math/SMatrix.h"

using namespace ReiToEngine;

RT_TEST(SMatrix_static_zero_identity){
    auto Z = SMatrix<3,3,float>::Zero();
    RT_ASSERT_TRUE(Z.IsZero());

    auto I = SMatrix<3,3,int>::Identity();
    RT_ASSERT_TRUE(I.IsIdentity());

    // Zero should be absorbing for addition with identity when multiplied by zero
    auto A = SMatrix<3,3,float>{1,2,3,4,5,6,7,8,9};
    auto R = A + SMatrix<3,3,float>::Zero();
    RT_ASSERT_TRUE(R == A);
}
