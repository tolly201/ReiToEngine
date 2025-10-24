#include "../Core/TestCore.h"
#include "L20_Platform/L24_Math/Include.h"
#include <type_traits>
#include <cstdint>

using namespace ReiToEngine;

RT_TEST_ST("Math", SVector4_copy_and_alignment){
    SVector4<float> a{1.f,2.f,3.f,4.f};
    SVector4<float> b = a; // copy
    RT_EXPECT_TRUE(b.x==1.f && b.y==2.f && b.z==3.f && b.w==4.f);

    auto* p = new SVector4<float>{5.f,6.f,7.f,8.f};
    RT_EXPECT_TRUE(p->x==5.f && p->y==6.f && p->z==7.f && p->w==8.f);
    auto addr = reinterpret_cast<std::uintptr_t>(p);
    // For float specialization, class has alignas(16)
    RT_EXPECT_TRUE(addr % 16 == 0);
    delete p;

    auto* arr = new SVector4<float>[2];
    arr[1] = SVector4<float>{9.f,10.f,11.f,12.f};
    auto addr0 = reinterpret_cast<std::uintptr_t>(&arr[0]);
    RT_EXPECT_TRUE(addr0 % 16 == 0);
    delete[] arr;
}

RT_TEST_ST("Math", Quaternion_copy_and_alignment){
    Quaternion<float> q{1.f,2.f,3.f,4.f};
    Quaternion<float> q2 = q;
    RT_EXPECT_TRUE(q2.x==1.f && q2.y==2.f && q2.z==3.f && q2.w==4.f);

    auto* pq = new Quaternion<float>{0.f,1.f,0.f,0.f};
    auto addr = reinterpret_cast<std::uintptr_t>(pq);
    RT_EXPECT_TRUE(addr % 16 == 0);
    delete pq;
}
