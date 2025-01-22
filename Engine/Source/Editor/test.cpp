#include <iostream>
#include "Core/Core.h"
// #include "stb/stb_image.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
int a1()
{
 std::cout<<1;
}
int main()
{
   a1();
    int* p = new int;
    delete p;
    int* q2 = new int[5];
    delete[] q2;
    void* q3 = malloc(20);
    free(q3);
    // int width, height, channels;
    // unsigned char *data = stbi_load("image.png", &width, &height, &channels, 0);

    // if (data) {
    //     // 处理图像数据...
    //     std::cout << "loaded";
    //     stbi_image_free(data); // 释放图像数据
    // }
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("nanosuit/nanosuit.obj",
                                             aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return 1;
    }
    return 0;
}