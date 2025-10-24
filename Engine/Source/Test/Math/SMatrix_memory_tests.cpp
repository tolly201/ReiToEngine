#include "../Core/TestCore.h"
#include "L20_Platform/L24_Math/Include.h"
#include <type_traits>
#include <cstdint>

using namespace ReiToEngine;

static_assert(std::is_copy_constructible_v<SMatrix<3,3,float>>, "SMatrix 3x3 float should be copy constructible");
static_assert(std::is_move_constructible_v<SMatrix<3,3,float>>, "SMatrix has defaulted move ctor");

RT_TEST_ST("Math", SMatrix_copy_move_and_assign){
    SMatrix<2,2,int> A{1,2,3,4};
    SMatrix<2,2,int> B{A}; // copy
    RT_EXPECT_TRUE(B.data[0]==1 && B.data[3]==4);

    SMatrix<2,2,int> C;
    C = A; // assign
    RT_EXPECT_TRUE(C.data[1]==2 && C.data[2]==3);

    SMatrix<2,2,int> D{std::move(A)}; // move should preserve values due to array copy
    RT_EXPECT_TRUE(D.data[0]==1 && D.data[3]==4);
}

RT_TEST_ST("Math", SMatrix_new_delete){
    auto* p = new SMatrix<3,3,float>{1,0,0, 0,1,0, 0,0,1};
    RT_EXPECT_TRUE(p->IsIdentity());
    auto addr = reinterpret_cast<std::uintptr_t>(p);
    RT_EXPECT_TRUE(addr % alignof(SMatrix<3,3,float>) == 0);
    delete p;

    auto* arr = new SMatrix<2,2,int>[3];
    arr[2] = SMatrix<2,2,int>{1,2,3,4};
    RT_EXPECT_TRUE(arr[2].data[0]==1 && arr[2].data[3]==4);
    delete[] arr;
}

RT_TEST_ST("Math", SMatrix_determinant_and_rank){
    // 2x2 det
    SMatrix<2,2,int> M2{1,2,3,4};
    RT_EXPECT_TRUE(M2.Determinant() == (1*4 - 2*3));

    // 3x3 det known example (row-major order)
    SMatrix<3,3,int> M3{ 6,1,1, 4,-2,5, 2,8,7 };
    // Our Determinant follows formula using data in row-major
    // Compute expected by direct expansion: 6*((-2)*7 - 5*8) - 1*(4*7 - 5*2) + 1*(4*8 - (-2)*2)
    int expected = 6*((-2)*7 - 5*8) - 1*(4*7 - 5*2) + 1*(4*8 - (-2)*2);
    RT_EXPECT_TRUE(M3.Determinant() == expected);

    // Rank tests
    SMatrix<3,3,float> Z{}; // zero matrix rank 0
    RT_EXPECT_TRUE(Z.Rank() == 0);

    SMatrix<3,3,float> R1{1,2,3, 2,4,6, 3,6,9}; // rows are multiples
    RT_EXPECT_TRUE(R1.Rank() == 1);
}
