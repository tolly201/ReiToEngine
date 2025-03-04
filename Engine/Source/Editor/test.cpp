#include <iostream>
#include "Core/Core.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <Function/Renderer/IRenderer.h>

//[capture variable]
//{accepted variable}
auto testVectors = []()
{
    ReiToEngine::Vec4d v1, v2;
    ReiToEngine::Vec4d v3 = v1.cross3D(v2);
    std::cout << &v1<< std::endl << &v2<< std::endl <<&v3<< std::endl;
    std::cout << v3.x<< std::endl << v3.y<< std::endl <<v3.z<< std::endl<<v3.w<< std::endl;
    v3 = v1.normalize();
    std::cout << v3;
    std::cout << v1;
    v1.normalizeSelf();
    std::cout << v1;
    v3 = v1.normalize();
    std::cout << v3;
    ReiToEngine::Vec4d v4({1,2,3,4});
    ReiToEngine::Vec4d v5(1,2,3,4);
    ReiToEngine::Vec4d v6(2);
    std::cout << v4;
    std::cout << v5;
    std::cout << v6;
    ReiToEngine::Vec4d v7(v4);
    ReiToEngine::Vec4d v8(std::move(v4));
    std::cout << v7;
    std::cout << v8;
    std::cout << v4;
    v4 = {3,3,3,3};
    std::cout << v4;
};

void testLamba()
{
    // 示例 1：简单的打印
    auto a = []() {
        std::cout << "Hello, Lambda!" << std::endl;
    };
    a();
    // 示例 2：捕获外部变量
    int x = 10;
    [&x]() {
        std::cout << "Captured x: " << x << std::endl;
    }();

    // 示例 3：带参数和返回值的 Lambda
    int result = [](int a, int b) -> int {
        return a + b;
    }(5, 3);
    std::cout << "Result: " << result << std::endl;

    // 示例 4：修改捕获的变量
    int y = 10;
    [y]() mutable {
        y = 20;
        std::cout << "Modified y: " << y << std::endl;
    }();
    std::cout << "Original y: " << y << std::endl; // 仍然是 10，因
}
int main()
{
    testVectors();
    testLamba();
    return 0;
}
