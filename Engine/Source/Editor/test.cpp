#include <_types/_uint32_t.h>
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/scene.h>        // Output data structure

#include <assimp/Importer.hpp>  // C++ importer interface
#include <cstddef>
#include <iostream>

#include "Platform/Application/Include/RTApplication.h"
#include "Launch/LaunchEngineLoop.cpp"
#include "Platform/HAL/System/Include/SystemInfo.h"
#include "Platform/HAL/Window/Include/IWindow.h"
#include "Core/Math/IMath.h"

#include "Function/RenderManager/Include/RenderManager.h"
#include "Platform/HAL/Window/Include/MACOSX/MacOSXWindow.h"
#include "Platform/ImageParser/Include/STBImageParser.h"

//[capture variable]
//{accepted variable}
auto testVectors = []() {
    ReiToEngine::Vec4d v1, v2;
    v1 = {1, 4, 5, 2};
    v2 = {4, 41, 10, 3};
    std::cout << v2;

    ReiToEngine::Vec4d v3 = v1.cross3D(v2);
    v3 = v1.normalize();
    v1.normalizeSelf();
    v3 = v1.normalize();
    ReiToEngine::Vec4d v4({1, 2, 3, 4});
    ReiToEngine::Vec4d v5({1, 2, 3, 4});
    ReiToEngine::Vec4d v6(2);
    ReiToEngine::Vec4d v7(v4);
    std::cout << v7;
    ReiToEngine::Vec4d v8(std::move(v4));
    std::cout << v7;
    std::cout << v8;
};

void testLamba() {
    // 示例 1：简单的打印
    auto a = []() { std::cout << "Hello, Lambda!" << std::endl; };
    a();
    // 示例 2：捕获外部变量
    int x = 10;
    [&x]() { std::cout << "Captured x: " << x << std::endl; }();

    // 示例 3：带参数和返回值的 Lambda
    int result = [](int a, int b) -> int { return a + b; }(5, 3);
    std::cout << "Result: " << result << std::endl;

    // 示例 4：修改捕获的变量
    int y = 10;
    [y]() mutable {
        y = 20;
        std::cout << "Modified y: " << y << std::endl;
    }();
    std::cout << "Original y: " << y << std::endl;  // 仍然是 10，因
}

void testMatrix() {
    ReiToEngine::Matrix4x4d m4 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    std::cout << m4[2];
    std::cout << m4[1][2];
}

int main(int argc, const char * argv[]) {
    // return 0;
    return RuntimeMainLoopEntry(argc, argv);
}
