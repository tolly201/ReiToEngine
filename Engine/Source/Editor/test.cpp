#include <iostream>
#include "Core/Core.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <Function/Renderer/IRenderer.h>
int main()
{
//     ReiToEngine::RTCFileManager m;
//     ReiToEngine::RTFArchive* ac = m.CreateDebugFileWriter("./test.txt");
//     bool result = ac->Seek(0,0);
//     std::cout <<result;
//     char* count = new char[4];
//     ac->Read(count, 4);
//     for (int i = 0; i < 4; ++i)
//     {
//         std::cout << count[i];
//     }
//     result = ac->Write("1234", 4);
//     std::cout <<result;
//     ac->Flush();
//     std::string str = "hello";
//     std::string other = std::move(str);
//     // 不要使用 str 的值，因为它处于未指定状态
//     std::cout << std::endl; // 输出 "world"
//     std::cout << str << std::endl; // 输出 "world"
//    std::cout << std::endl; // 输出 "world"
//     std::cout << other << std::endl; // 输出 "world"
//     str = "world"; // 现在 str 重新赋值，可以安全地使用
//     std::cout << str << std::endl; // 输出 "world"

//     str.clear(); // 清空 str，使其处于已知状态
    int h = 1600;
    int w = 2560;
    int channels = 4;
    ReiToEngine::STBImageParser imageParser;
    ReiToEngine::SimpleRenderer r;
    size_t RBO = r.CreateBuffer(w,h,channels);
    ReiToEngine::Image image(w,h,channels);
    r.SetBackColor(RBO, 0,0,0,255);

    ReiToEngine::Vec3i v0, v1, v2, color;

    color.x = 255;
    color.y = 0;
    color.z = 0;
    v0.x = 20;
    v0.y = 200;
    v0.z = 10;
    v1.x = 200;
    v1.y = 200;
    v1.z = 30;
    v2.x = 20;
    v2.y = 20;
    v2.z = -10;
    r.DrawTriangle(RBO, v0, v1,v2,color);

    v0.x = 20;
    v0.y = 200;
    v0.z = 0;
    v1.x = 200;
    v1.y = 200;
    v1.z = 70;
    v2.x = 20;
    v2.y = 20;
    v2.z = 30;
    color.x = 0;
    color.y = 255;
    color.z = 0;
    r.DrawTriangle(RBO, v0, v1,v2, color);

    uint8_t* data = r.GetBuffer(RBO);
    image.SetData(data);
    image.SetType(ReiToEngine::EImageType::IMAGE_PNG);

    imageParser.Write("./test1.png",image);
    int a;
    return 0;
}
