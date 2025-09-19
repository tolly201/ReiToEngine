#include "MathTest.h"
#include "L60_Core/Math/SMatrix.h"

using namespace ReiToEngine;

RT_TEST(SMatrix_add_mul){
    SMatrix<3,3,float> A{ 1,2,3, 4,5,6, 7,8,9 };
    SMatrix<3,3,float> B{ 9,8,7, 6,5,4, 3,2,1 };

    auto C = A + B;
    RT_ASSERT_TRUE(C.data[0]==10 && C.data[8]==10);

    auto S = A * 2.0f;
    RT_ASSERT_TRUE(S.data[0]==2 && S.data[8]==18);
}

RT_TEST(SMatrix_transpose_identity){
    SMatrix<2,2,int> I{1,0,0,1};
    auto T = I.transpose();
    RT_ASSERT_TRUE(T == I);
    RT_ASSERT_TRUE(I.IsIdentity());
}
