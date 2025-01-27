#include <iostream>
#include "Core/Core.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
int main()
{
    ReiToEngine::RTCFileManager m;
    ReiToEngine::RTFArchive* ac = m.CreateDebugFileWriter("./test.txt");
    bool result = ac->Seek(0,0);
    std::cout <<result;
    char* count = new char[4];
    ac->Read(count, 4);
    for (int i = 0; i < 4; ++i)
    {
        std::cout << count[i];
    }
    result = ac->Write("1234", 4);
    std::cout <<result;
    ac->Flush();
    std::string str = "hello";
    std::string other = std::move(str);
    // 不要使用 str 的值，因为它处于未指定状态
    std::cout << std::endl; // 输出 "world"  
    std::cout << str << std::endl; // 输出 "world" 
   std::cout << std::endl; // 输出 "world"  
    std::cout << other << std::endl; // 输出 "world" 
    str = "world"; // 现在 str 重新赋值，可以安全地使用
    std::cout << str << std::endl; // 输出 "world"

    str.clear(); // 清空 str，使其处于已知状态

    return 0;

}