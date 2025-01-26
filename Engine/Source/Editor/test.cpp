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
    return 0;
}