#include "../Core/TestCore.h"
#include "L20_Platform/L24_Math/Include.h"

using namespace ReiToEngine;

RT_TEST_ST("Math", SMatrix_add_mul){
    SMatrix<3,3,float> A{ 1,2,3, 4,5,6, 7,8,9 };
    SMatrix<3,3,float> B{ 9,8,7, 6,5,4, 3,2,1 };

    auto C = A + B;
    RT_EXPECT_TRUE(C.data[0]==10 && C.data[8]==10);

    auto S = A * 2.0f;
    RT_EXPECT_TRUE(S.data[0]==2 && S.data[8]==18);
}

RT_TEST_ST("Math", SMatrix_transpose_identity){
    SMatrix<2,2,int> I{1,0,0,1};
    auto T = I.transpose();
    RT_EXPECT_TRUE(T == I);
    RT_EXPECT_TRUE(I.IsIdentity());
}
