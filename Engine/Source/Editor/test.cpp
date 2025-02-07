#include <iostream>
#include "Core/Core.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

void line(int x0, int y0, int x1, int y1, ReiToEngine::Image &image, ReiToEngine::IColor &color)
{
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.SetColor(y, x, color); 
        } else { 
            image.SetColor(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    }
}

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

    ReiToEngine::STBImageParser imageParser;

    ReiToEngine::Image image(2560,1600,4);
    for (int i = 0; i < 2560; ++i)
        for(int j = 0; j < 1600; ++j)
        {
            ReiToEngine::IColor color;
            color.r = 0;
            color.g = 0;
            color.b = 0;
            color.a = 255;
            image.SetColor(i, j, color);
        }
    ReiToEngine::IColor color;
    color.r = 255;
    color.g = 0;
    color.b = 0;
    color.a = 255;
    line(2, 2, 1000, 1000, image, color);    
    image.SetType(ReiToEngine::EImageType::IMAGE_PNG);

    imageParser.Write("./test1.png",image);
    int a;
    return 0;
}