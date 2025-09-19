#include "MathTest.h"
#include "L60_Core/Math/Quaternion.h"

using namespace ReiToEngine;

RT_TEST(Quaternion_normalize_and_distance){
    Quaternion<float> q{1,2,3,4};
    auto n = q.normalize();
    RT_ASSERT_NEAR(n.length(), 1.f, 1e-5f);

    Quaternion<float> q2{1,2,3,5};
    auto d = q.distance(q2);
    RT_ASSERT_TRUE(d > 0.f);
}
