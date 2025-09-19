#include "MathTest.h"
#include "L60_Core/Math/VectorInclude.h"
#include <cmath>

using namespace ReiToEngine;

RT_TEST(SVector_basic_ops){
    Vec3f a{1.f,2.f,3.f};
    Vec3f b{3.f,2.f,1.f};

    auto c = a + b;
    RT_ASSERT_TRUE(c[0]==4.f && c[1]==4.f && c[2]==4.f);

    auto d = a - b;
    RT_ASSERT_TRUE(d[0]==-2.f && d[1]==0.f && d[2]==2.f);

    auto e = a * 2.f;
    RT_ASSERT_TRUE(e[0]==2.f && e[1]==4.f && e[2]==6.f);

    float dot = a*b;
    RT_ASSERT_TRUE(dot == 1*3 + 2*2 + 3*1);
}

RT_TEST(SVector_normalize){
    Vec3f a{3.f,0.f,4.f};
    auto na = a.normalize();
    RT_ASSERT_NEAR(na.length(), 1.f, 1e-5f);

    Vec3f z{0.f,0.f,0.f};
    auto nz = z.normalize();
    RT_ASSERT_TRUE(nz.isZero());
}
