#include <iostream>
#include "Core/Core.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <Function/Renderer/SimpleRenderer.h>
#include "Core/Math/IMath.h"
#include "Camera.h"
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
    ReiToEngine::Camera camera = ReiToEngine::Camera(ReiToEngine::Vec3d({0,0, 10}));
    int a;
    ReiToEngine::SimpleRenderer render;
    size_t width = 400;
    size_t height = 300;

    size_t SBO = render.CreateSceneInfo();
    size_t FBO = render.CreateFrameBuffer(width, height, 4);
    ReiToEngine::Vec3d* triangle = new ReiToEngine::Vec3d[3];
    ReiToEngine::Vec4d *colors = new ReiToEngine::Vec4d[3];
    uint32_t* line = new uint32_t[3];
    triangle[0] = ReiToEngine::Vec3d({0, 0, 0});
    triangle[1] = ReiToEngine::Vec3d({0, 100, 0});
    triangle[2] = ReiToEngine::Vec3d({100, 0, 0});

    colors[0] = ReiToEngine::Vec4d({255, 0, 0, 255});
    colors[1] = ReiToEngine::Vec4d({0, 255, 0, 255});
    colors[2] = ReiToEngine::Vec4d({0, 0, 255, 255});
    line[0] = 0;
    line[1] = 1;
    line[2] = 2;
    ReiToEngine::Matrix4x4d model = ReiToEngine::Matrix4x4d({1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1});
    ReiToEngine::Matrix4x4d View = camera.GetViewMatrix();
    ReiToEngine::Matrix4x4d Projection = camera.GetProjectionMatrix();
    ReiToEngine::Matrix4x4d NDC = camera.GetNDCMatrix(width, height);
    std::cout << "transform: \n" << model;
    std::cout << "View: \n" << View;
    std::cout << "Projection: \n" << Projection;

    size_t VBO = render.CreateVertexBuffer(triangle, nullptr, nullptr, colors, 3);
    size_t IBO = render.CreateIndiceBuffer(line, 3, false);
    size_t MBO = render.CreateMatrixRenderUnit();
    render.AppendMatrix(MBO, model);
    render.AppendMatrix(MBO, View);
    render.AppendMatrix(MBO, Projection);
    render.AppendMatrix(MBO, NDC);

    render.BindFrameBuffer(SBO, FBO);
    render.BindObject(SBO, VBO, IBO);
    render.BindMatrix(SBO, MBO);
    uint8_t* data;
    size_t size;
    render.DrawFrame(SBO, data, size);
    ReiToEngine::STBImageParser imageParser;
    ReiToEngine::Image image(width,height,4);
    image.SetData(data);
    image.SetType(ReiToEngine::EImageType::IMAGE_PNG);

    imageParser.Write("./test1.png",image);
}

void testCamera()
{
    ReiToEngine::Camera camera = ReiToEngine::Camera(ReiToEngine::Vec3d({0,0, 10}));
    int a;
    int width = 800;
    int height = 600;
    uint32_t* line = new uint32_t[3];
    ReiToEngine::Vec4d* triangle = new ReiToEngine::Vec4d[3];
    triangle[0] = ReiToEngine::Vec4d({0, 0, 0, 1});
    triangle[1] = ReiToEngine::Vec4d({0, 5, 0, 1});
    triangle[2] = ReiToEngine::Vec4d({5, 0, 0, 1});
    ReiToEngine::Matrix4x4d View = camera.GetViewMatrix();
    ReiToEngine::Matrix4x4d Projection = camera.GetProjectionMatrix();
    ReiToEngine::Matrix4x4d trans = View * Projection;
    ReiToEngine::Matrix4x4d NDC = camera.GetNDCMatrix(width, height);
    ReiToEngine::Matrix4x4d screen = camera.GetScreenMatrix(width, height);
    std::cout << "start computing:\n";
    std::cout << View * Projection;

    ReiToEngine::Matrix4x4d trans1 = Projection * View;
    ReiToEngine::Vec4d transformed_v0,transformed_v1,transformed_v2;

    std::cout << "View computing:\n";
    std::cout << View;
    std::cout << std::endl;

    std::cout << triangle[0];
    std::cout << triangle[1];
    std::cout << triangle[2];
    std::cout << std::endl;

    transformed_v0 = View * triangle[0] ;
    transformed_v1 = View * triangle[1] ;
    transformed_v2 = View * triangle[2] ;

    std::cout << transformed_v0;
    std::cout << transformed_v1;
    std::cout << transformed_v2;
    std::cout << "View computing end:\n";

    std::cout << "Projection computing:\n";
    std::cout << Projection;
    std::cout << std::endl;
    std::cout << transformed_v0;
    std::cout << transformed_v1;
    std::cout << transformed_v2;
    std::cout << std::endl;

    transformed_v0 = Projection * transformed_v0;
    transformed_v1 = Projection * transformed_v1 ;
    transformed_v2 = Projection * transformed_v2 ;

    std::cout << transformed_v0;
    std::cout << transformed_v1;
    std::cout << transformed_v2;
    std::cout << std::endl;

    std::cout << "Projection computing end:\n";

    std::cout << "NDC computing:\n";
    std::cout << NDC;
    std::cout << std::endl;
    std::cout << transformed_v0;
    std::cout << transformed_v1;
    std::cout << transformed_v2;
    std::cout << std::endl;

    transformed_v0 = NDC * transformed_v0 ;
    transformed_v1 = NDC * transformed_v1 ;
    transformed_v2 = NDC * transformed_v2 ;

    std::cout << transformed_v0;
    std::cout << transformed_v1;
    std::cout << transformed_v2;
    std::cout << std::endl;


    std::cout << "NDC computing end:\n";

    transformed_v0 = transformed_v0 / transformed_v0.w;
    transformed_v1 = transformed_v1 / transformed_v1.w;
    transformed_v2 = transformed_v2 / transformed_v2.w;

    std::cout << transformed_v0;
    std::cout << transformed_v1;
    std::cout << transformed_v2;
    std::cout << std::endl;

    std::cout << screen * transformed_v0;
    std::cout << screen * transformed_v1;
    std::cout << screen * transformed_v2;
    std::cout << std::endl;

    ReiToEngine::Matrix4x4d m = NDC * Projection * View;


    transformed_v0 = m * triangle[0] ;
    transformed_v1 = m * triangle[1] ;
    transformed_v2 = m * triangle[2] ;

    transformed_v0 = transformed_v0 / transformed_v0.w;
    transformed_v1 = transformed_v1 / transformed_v1.w;
    transformed_v2 = transformed_v2 / transformed_v2.w;

    std::cout << screen * transformed_v0;
    std::cout << screen * transformed_v1;
    std::cout << screen * transformed_v2;
    std::cout << std::endl;
}

int main()
{
    // testVectors();
    // testLamba();
    // testMatrix();
    testRender();
    // testCamera();
    return 0;
}
