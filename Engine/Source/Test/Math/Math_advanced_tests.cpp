#include "../Core/TestCore.h"
#include "L60_Core/Math/SVector3.h"
#include "L60_Core/Math/SMatrix.h"
#include "L60_Core/Math/Quaternion.h"
#include <cmath>

using namespace ReiToEngine;

// 统一辅助比较
static bool near(double a, double b, double eps=1e-9){ return std::fabs(a-b) <= eps; }

// ---- SVector3 高级行为 ----
RT_TEST_ST("Math", SVector3_normalize_zero, "math-advanced"){
    SVector3<double> z{0.0,0.0,0.0};
    auto n = z.normalize();
    RT_EXPECT_TRUE(n.isZero());
    z.normalizeSelf();
    RT_EXPECT_TRUE(z.isZero());
}

RT_TEST_ST("Math", SVector3_trig_clamp_asin_acos, "math-advanced"){
    SVector3<double> a{2.0,-2.0,0.5}; // asin/acos 会 clamp
    auto as = a.asin();
    auto ac = a.acos();
    // clamp 后 asin(±1)=±pi/2, acos(±1)=0 或 pi
    constexpr double PI = 3.14159265358979323846;
    RT_EXPECT_NEAR(as.x,  PI/2, 1e-12);
    RT_EXPECT_NEAR(as.y, -PI/2, 1e-12);
    RT_EXPECT_NEAR(ac.x, 0.0,   1e-12); // acos(1)=0
    RT_EXPECT_NEAR(ac.y, PI,    1e-12); // acos(-1)=pi
}

RT_TEST_ST("Math", SVector3_reflect_project_relations, "math-advanced"){
    SVector3<double> v{1.0, -2.0, 0.5};
    SVector3<double> n{0.0, 1.0, 0.0}; // y 方向法线
    auto r = v.reflect(n); // v - 2*(v·n)*n
    // v.y = -2 => v·n = -2 => r = (1, -2, 0.5) -2*(-2)*(0,1,0) => (1, -2 +4, 0.5)=(1,2,0.5)
    RT_EXPECT_NEAR(r.y, 2.0, 1e-12);
    RT_EXPECT_NEAR(r.x, 1.0, 1e-12);

    auto p = v.project(n); // (v·n / |n|^2) * n = (-2)*n
    RT_EXPECT_NEAR(p.x, 0.0, 1e-12);
    RT_EXPECT_NEAR(p.y, -2.0, 1e-12);
    RT_EXPECT_NEAR(p.z, 0.0, 1e-12);
}

RT_TEST_ST("Math", SVector3_cross_parallel_zero, "math-advanced"){
    SVector3<double> a{1,2,3};
    SVector3<double> b{2,4,6}; // 平行 (b = 2a)
    auto c = a.cross3D(b);
    RT_EXPECT_TRUE(c.isZero());
}

RT_TEST_ST("Math", SVector3_lerp_edge_cases, "math-advanced"){
    SVector3<double> a{1,1,1};
    SVector3<double> b{3,5,7};
    auto L0 = a.lerp(b, 0.0);
    auto L1 = a.lerp(b, 1.0);
    auto Lh = a.lerp(b, 0.5);
    RT_EXPECT_TRUE(L0 == a);
    RT_EXPECT_TRUE(L1 == b);
    RT_EXPECT_NEAR(Lh.x, 2.0, 1e-12);
    RT_EXPECT_NEAR(Lh.y, 3.0, 1e-12);
    RT_EXPECT_NEAR(Lh.z, 4.0, 1e-12);
}

// ---- Quaternion 高级行为 ----
RT_TEST_ST("Math", Quaternion_inverse_tiny_norm_stable, "math-advanced"){
    // 极小长度：防止除以非常小的值导致巨大数值
    Quaternion<double> q{1e-15, -1e-15, 2e-15, 1e-15};
    auto inv = q.inverse();
    // q * inv 应接近单位 (但数值误差较大，放宽容差)
    auto id = q * inv;
    RT_EXPECT_NEAR(id.x, 0.0, 1e-6);
    RT_EXPECT_NEAR(id.y, 0.0, 1e-6);
    RT_EXPECT_NEAR(id.z, 0.0, 1e-6);
    RT_EXPECT_NEAR(id.w, 1.0, 1e-6);
}

RT_TEST_ST("Math", Quaternion_chain_rotation_identity, "math-advanced"){
    // 随意构造两个四元数（非单位），归一化后测试组合 * 逆
    Quaternion<double> q1{0.3, -0.5, 0.7, 1.2};
    Quaternion<double> q2{-0.6, 0.4, 0.2, 0.9};
    auto i1 = q1.inverse();
    auto i2 = q2.inverse();
    auto combo = q1 * q2;
    auto id = combo * (i2 * i1); // (q1 q2)(q2^{-1} q1^{-1}) = 1
    RT_EXPECT_NEAR(id.x, 0.0, 1e-9);
    RT_EXPECT_NEAR(id.y, 0.0, 1e-9);
    RT_EXPECT_NEAR(id.z, 0.0, 1e-9);
    RT_EXPECT_NEAR(id.w, 1.0, 1e-9);
}

// ---- Matrix 高级行为 ----
RT_TEST_ST("Math", SMatrix_transpose_involution, "math-advanced"){
    SMatrix<3,3,double> A{ 1,2,3, 4,5,6, 7,8,10 };
    auto AT = A.transpose();
    auto ATT = AT.transpose();
    for(int i=0;i<9;++i) RT_EXPECT_NEAR(ATT.data[i], A.data[i], 1e-12);
}

RT_TEST_ST("Math", SMatrix_inverse_4x4_identity_check, "math-advanced"){
    SMatrix<4,4,double> A{ 4,0,0,0,
                           0,5,0,0,
                           0,0,6,0,
                           0,0,0,7};
    auto inv = A.Inverse();
    auto I = A * inv;
    // 对角对 1，其余 0
    double diagExpected[4] = {1,1,1,1};
    for(int r=0;r<4;++r){
        for(int c=0;c<4;++c){
            double v = I.data[r*4+c];
            if(r==c) RT_EXPECT_NEAR(v, diagExpected[r], 1e-12);
            else RT_EXPECT_NEAR(v, 0.0, 1e-12);
        }
    }
}

RT_TEST_ST("Math", SMatrix_inverse_4x4_singular_behavior, "math-advanced"){
    // 构造奇异 4x4（第二行=第一行*2）
    SMatrix<4,4,double> S{ 1,2,3,4,
                           2,4,6,8,
                           0,1,0,1,
                           0,0,0,0 };
    auto inv = S.Inverse();
    // 预期实现返回零矩阵（与 3x3 singular 用例一致）
    bool allZero = true; for(double d : inv.data){ if(std::fabs(d) > 1e-12){ allZero=false; break; } }
    RT_EXPECT_TRUE(allZero);
}

RT_TEST_ST("Math", SMatrix_mul_inverse_det_relation, "math-advanced"){
    // A 与 B 的逆存在：这里选对角+轻微偏移避免奇异
    SMatrix<2,2,double> A{3,1, 0,2};
    SMatrix<2,2,double> B{2,0, 1,4};
    auto invA = A.Inverse();
    auto invB = B.Inverse();
    auto AB = A * B;
    auto invAB = AB.Inverse();
    // (AB)^{-1} == B^{-1} A^{-1}
    auto C = invB * invA;
    for(int i=0;i<4;++i) RT_EXPECT_NEAR(invAB.data[i], C.data[i], 1e-9);
}
