#include "../Core/TestCore.h"
#include "L60_Core/Math/VectorInclude.h"
#include <stdexcept>

using namespace ReiToEngine;

RT_TEST_ST("Math", SVector_dot_properties){
    Vec3f a{1.f,2.f,3.f};
    Vec3f b{-2.f,0.5f,4.f};
    Vec3f c{0.5f,-1.f,2.f};

    float ab = a.dot(b);
    float ba = b.dot(a);
    RT_EXPECT_TRUE(std::fabs(ab - ba) < 1e-6f);

    float a_bPlusc = a.dot(b + c);
    float a_b = a.dot(b);
    float a_c = a.dot(c);
    RT_EXPECT_TRUE(std::fabs(a_bPlusc - (a_b + a_c)) < 1e-6f);
}

RT_TEST_ST("Math", SVector_reflect_project){
    Vec3f v{1.f,-1.f,0.f};
    Vec3f n{0.f,1.f,0.f}; // y-axis normal, already normalized
    auto r = v.reflect(n);
    RT_EXPECT_TRUE(std::fabs(r[0]-1.f) < 1e-6f && std::fabs(r[1]-1.f) < 1e-6f && std::fabs(r[2]-0.f) < 1e-6f);

    Vec3f p = Vec3f{3.f,4.f,0.f}.project(Vec3f{1.f,0.f,0.f});
    RT_EXPECT_TRUE(std::fabs(p[0]-3.f) < 1e-6f && std::fabs(p[1]-0.f) < 1e-6f && std::fabs(p[2]-0.f) < 1e-6f);
}

RT_TEST_ST("Math", SVector_normalize_properties){
    Vec3f a{3.f,4.f,0.f};
    auto an = a.normalize();
    RT_EXPECT_TRUE(an.isNormalized());

    Vec3f zero{0.f,0.f,0.f};
    auto zn = zero.normalize();
    RT_EXPECT_TRUE(zn.isZero());
}

RT_TEST_ST("Math", SVector_index_bounds){
    Vec3f a{1.f,2.f,3.f};
    bool threw_low = false, threw_high = false;
    try { (void)a[-1]; } catch(const std::out_of_range&) { threw_low = true; }
    try { (void)a[3]; } catch(const std::out_of_range&) { threw_high = true; }
    RT_EXPECT_TRUE(threw_low && threw_high);
}
