#include "../Core/TestCore.h"
#include "L60_Core/Math/SMatrix.h"
#include "L60_Core/Math/VectorInclude.h"

using namespace ReiToEngine;

RT_TEST_ST("Math", SMatrix_multiply_identity){
    SMatrix<3,3,float> I{1,0,0, 0,1,0, 0,0,1};
    SMatrix<3,3,float> A{1,2,3, 4,5,6, 7,8,9};
    auto L = I * A;
    auto R = A * I;
    RT_EXPECT_TRUE(L == A && R == A);
}

RT_TEST_ST("Math", SMatrix_transform_vector){
    SMatrix<3,3,float> A{1,2,3, 4,5,6, 7,8,9};
    Vec3f v{1.f, 0.f, -1.f};
    auto out = A * v; // uses operator*(Vector)
    // Row-major multiply check
    RT_EXPECT_TRUE(std::fabs(out[0] - (1*1 + 2*0 + 3*(-1))) < 1e-6f);
    RT_EXPECT_TRUE(std::fabs(out[1] - (4*1 + 5*0 + 6*(-1))) < 1e-6f);
    RT_EXPECT_TRUE(std::fabs(out[2] - (7*1 + 8*0 + 9*(-1))) < 1e-6f);
}

RT_TEST_ST("Math", SMatrix_is_zero_identity){
    SMatrix<2,2,int> Z{0,0,0,0};
    RT_EXPECT_TRUE(Z.IsZero());
    SMatrix<2,2,int> I{1,0,0,1};
    RT_EXPECT_TRUE(I.IsIdentity());
}
