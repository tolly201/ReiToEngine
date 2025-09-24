#include "../Core/TestCore.h"
#include "L60_Core/Math/VectorInclude.h"
#include "L60_Core/Math/SMatrix.h"
#include <cmath>

using namespace ReiToEngine;

RT_TEST_ST("Math", SVector_helpers){
    Vec3f a{1.f, -2.f, 3.f};
    Vec3f b{2.f, 0.5f, -4.f};

    auto h = a.hadamard(b);
    RT_EXPECT_TRUE(h[0]==2.f && h[1]==-1.f && h[2]==-12.f);

    auto c = a.clamp(-1.f, 2.f);
    RT_EXPECT_TRUE(c[0]==1.f && c[1]==-1.f && c[2]==2.f);

    auto angle = a.angle(b);
    RT_EXPECT_TRUE(angle >= 0.f && angle <= 3.1415927f);

    Vec3f a2 = a;
    RT_EXPECT_TRUE(a.approxEqual(a2));
    RT_EXPECT_TRUE(a.isFinite());
}

RT_TEST_ST("Math", SMatrix_row_col_trace){
    SMatrix<3,3,float> A{1,2,3, 4,5,6, 7,8,9};
    RT_EXPECT_TRUE(A.Trace() == 1+5+9);

    auto r1 = A.GetRow(1);
    RT_EXPECT_TRUE(r1[0]==4 && r1[1]==5 && r1[2]==6);

    auto c2 = A.GetColumn(2);
    RT_EXPECT_TRUE(c2[0]==3 && c2[1]==6 && c2[2]==9);

    Vec3f newr{9,8,7};
    A.SetRow(0, newr);
    auto r0 = A.GetRow(0);
    RT_EXPECT_TRUE(r0[0]==9 && r0[1]==8 && r0[2]==7);

    Vec3f newc{1,2,3};
    A.SetColumn(1, newc);
    auto c1 = A.GetColumn(1);
    RT_EXPECT_TRUE(c1[0]==1 && c1[1]==2 && c1[2]==3);
}
