#include "../Core/TestCore.h"
#include "L20_Platform/L24_Math/Include.h"
#include <cmath>

using namespace ReiToEngine;

RT_TEST_ST("Math", Quaternion_conjugate_inverse){
    Quaternion<double> q{1.0, 2.0, 3.0, 4.0};
    auto qc = q.conjugate();
    RT_EXPECT_TRUE(qc.x==-1.0 && qc.y==-2.0 && qc.z==-3.0 && qc.w==4.0);

    auto inv = q.inverse();
    // q * q^{-1} ≈ 1 (单位元)
    auto id = q * inv;
    RT_EXPECT_NEAR(id.x, 0.0, 1e-9);
    RT_EXPECT_NEAR(id.y, 0.0, 1e-9);
    RT_EXPECT_NEAR(id.z, 0.0, 1e-9);
    RT_EXPECT_NEAR(id.w, 1.0, 1e-9);
}

RT_TEST_ST("Math", Quaternion_hamilton_product_unit){
    // 单位四元数的逆 = 共轭
    Quaternion<double> u{0.0, 0.0, 0.0, 1.0};
    auto invu = u.inverse();
    RT_EXPECT_TRUE(invu.x==0.0 && invu.y==0.0 && invu.z==0.0 && invu.w==1.0);

    // 乘法单位元性质：u * q = q * u = q
    Quaternion<double> q{0.5, -1.0, 2.0, 0.25};
    auto lhs = u * q;
    auto rhs = q * u;
    RT_EXPECT_NEAR(lhs.x, q.x, 1e-12); RT_EXPECT_NEAR(lhs.y, q.y, 1e-12);
    RT_EXPECT_NEAR(lhs.z, q.z, 1e-12); RT_EXPECT_NEAR(lhs.w, q.w, 1e-12);
    RT_EXPECT_NEAR(rhs.x, q.x, 1e-12); RT_EXPECT_NEAR(rhs.y, q.y, 1e-12);
    RT_EXPECT_NEAR(rhs.z, q.z, 1e-12); RT_EXPECT_NEAR(rhs.w, q.w, 1e-12);
}
