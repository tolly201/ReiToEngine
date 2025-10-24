#include "../Core/TestCore.h"
#include "L20_Platform/L24_Math/Include.h"
using namespace ReiToEngine;

RT_TEST_ST("Math", Quaternion_default_and_copy){
    Quaternion<float> q{};
    RT_EXPECT_TRUE(q.x==0 && q.y==0 && q.z==0 && q.w==0);
    q = Quaternion<float>{1,2,3,4};
    RT_EXPECT_TRUE(q.x==1 && q.y==2 && q.z==3 && q.w==4);
}
