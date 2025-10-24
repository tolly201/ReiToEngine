#include "../Core/TestCore.h"
#include "L20_Platform/L24_Math/Include.h"
#include <cmath>

using namespace ReiToEngine;

RT_TEST_ST("Math", SVector_basic_ops){
    Vec3f a{1.f,2.f,3.f};
    Vec3f b{3.f,2.f,1.f};

    auto c = a + b;
    RT_EXPECT_TRUE(c[0]==4.f && c[1]==4.f && c[2]==4.f);

    auto d = a - b;
    RT_EXPECT_TRUE(d[0]==-2.f && d[1]==0.f && d[2]==2.f);

    auto e = a * 2.f;
    RT_EXPECT_TRUE(e[0]==2.f && e[1]==4.f && e[2]==6.f);

    float dot = a*b;
    RT_EXPECT_TRUE(dot == 1*3 + 2*2 + 3*1);
}

RT_TEST_ST("Math", SVector_normalize){
    Vec3f a{3.f,0.f,4.f};
    auto na = a.normalize();
    RT_EXPECT_NEAR(na.length(), 1.f, 1e-5f);

    Vec3f z{0.f,0.f,0.f};
    auto nz = z.normalize();
    RT_EXPECT_TRUE(nz.isZero());
}
