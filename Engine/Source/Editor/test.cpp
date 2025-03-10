#include <iostream>
#include "Core/Core.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <Function/Renderer/SimpleRenderer.h>
#include "Core/Math/IMath.h"
//[capture variable]
//{accepted variable}
auto testVectors = []()
{
    ReiToEngine::Vec4d v1, v2;
    v1 = {1,4,5,2};
    v2 = {4,41,10,3};
    std::cout << v2;

    ReiToEngine::Vec4d v3 = v1.cross3D(v2);
    v3 = v1.normalize();
    v1.normalizeSelf();
    v3 = v1.normalize();
    ReiToEngine::Vec4d v4({1,2,3,4});
    ReiToEngine::Vec4d v5({1,2,3,4});
    ReiToEngine::Vec4d v6(2);
    ReiToEngine::Vec4d v7(v4);
    std::cout << v7;
    ReiToEngine::Vec4d v8(std::move(v4));
    std::cout << v7;
    std::cout << v8;
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

void testMatrix()
{
    ReiToEngine::Matrix4x4d m4 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    std::cout << m4[2];
    std::cout << m4[1][2];
}

void testRender()
{
    ReiToEngine::SimpleRenderer render;

    size_t SBO = render.CreateSceneInfo();
    size_t FBO = render.CreateFrameBuffer(64, 64, 4);
    ReiToEngine::Vec3d* triangle = new ReiToEngine::Vec3d[3];
    ReiToEngine::Vec4d *colors = new ReiToEngine::Vec4d[3];
    uint32_t* line = new uint32_t[6];
    triangle[0] = ReiToEngine::Vec3d({32, 32, 0});
    triangle[1] = ReiToEngine::Vec3d({40, 32, 0});
    triangle[2] = ReiToEngine::Vec3d({32, 40, 0});

    colors[0] = ReiToEngine::Vec4d({255, 0, 0, 255});
    colors[1] = ReiToEngine::Vec4d({0, 255, 0, 255});
    colors[2] = ReiToEngine::Vec4d({0, 0, 255, 255});
    line[0] = 0;
    line[1] = 1;
    line[2] = 0;
    line[3] = 2;
    line[4] = 1;
    line[5] = 2;
    ReiToEngine::Matrix4x4d transform = ReiToEngine::Matrix4x4d({1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
    size_t VBO = render.CreateVertexBuffer(triangle, nullptr, nullptr, colors, 3);
    size_t IBO = render.CreateIndiceBuffer(line, 6);
    size_t MBO = render.CreateMatrixRenderUnit();
    render.AppendMatrix(MBO, transform);
    render.BindFrameBuffer(SBO, FBO);
    render.BindObject(SBO, VBO, IBO);
    render.BindMatrix(SBO, MBO);
    uint8_t* data;
    size_t size;
    render.DrawFrame(SBO, data, size);
    ReiToEngine::STBImageParser imageParser;
    ReiToEngine::Image image(64,64,4);
    image.SetData(data);
    image.SetType(ReiToEngine::EImageType::IMAGE_PNG);

    imageParser.Write("./test1.png",image);
}

int main()
{
    // testVectors();
    // testLamba();
    // testMatrix();
    testRender();
    return 0;
}
