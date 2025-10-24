#include "../Core/TestCore.h"
#include "L20_Platform/L24_Math/Include.h"
#include <type_traits>
#include <cstdint>

using namespace ReiToEngine;

// Compile-time traits
static_assert(std::is_copy_constructible_v<Vec3f>, "Vec3f should be copy constructible");
static_assert(std::is_move_constructible_v<Vec3f>, "Vec3f should be move constructible (implicitly)");
static_assert(std::is_copy_assignable_v<Vec3f>, "Vec3f should be copy assignable");

RT_TEST_ST("Math", SVector_copy_ctor_and_assign){
    Vec3f a{1.f,2.f,3.f};
    Vec3f b{a};
    RT_EXPECT_TRUE(b[0]==1.f && b[1]==2.f && b[2]==3.f);

    Vec3f c{};
    c = a;
    RT_EXPECT_TRUE(c[0]==1.f && c[1]==2.f && c[2]==3.f);
}

RT_TEST_ST("Math", SVector_new_delete_alignment){
    // new/delete
    auto* p = new Vec3f{4.f,5.f,6.f};
    RT_EXPECT_TRUE(p->x==4.f && p->y==5.f && p->z==6.f);
    // alignment should satisfy alignof(Vec3f) at least
    auto addr = reinterpret_cast<std::uintptr_t>(p);
    RT_EXPECT_TRUE(addr % alignof(Vec3f) == 0);
    delete p;

    // new[]/delete[]
    constexpr int N = 4;
    auto* arr = new Vec3f[N];
    arr[2] = Vec3f{7.f,8.f,9.f};
    RT_EXPECT_TRUE(arr[2][0]==7.f && arr[2][1]==8.f && arr[2][2]==9.f);
    auto addr0 = reinterpret_cast<std::uintptr_t>(&arr[0]);
    RT_EXPECT_TRUE(addr0 % alignof(Vec3f) == 0);
    delete[] arr;
}
