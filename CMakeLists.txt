cmake_minimum_required(VERSION 3.20)
project(ReiToEngine)
# 通用编译设置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# 针对不同编译器的额外设置
if(MSVC)
else()
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
endif()

find_package(Vulkan REQUIRED)
# 资源文件输出目录
set(RESOURCE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/res)
# 引擎目录设置
set(ENGINE_ROOT_DIR Engine)

# 包含第三方库和引擎代码
include_directories(
    ${ENGINE_ROOT_DIR}/Thirdparty/stb/
)
include_directories(
    ${ENGINE_ROOT_DIR}/Source/Runtime/
)
include_directories(
    ${ENGINE_ROOT_DIR}/Thirdparty/Assimp/include/
)

include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(Engine/Source/Runtime)

# 设置 Assimp 的构建选项
set(ASSIMP_BUILD_TESTS OFF)        # 禁用测试
set(ASSIMP_BUILD_SAMPLES OFF)      # 禁用示例
set(ASSIMP_BUILD_DOCS OFF)         # 禁用文档
set(ASSIMP_WARNINGS_AS_ERRORS OFF)

# add_subdirectory(Engine/Thirdparty/Assimp)
add_subdirectory(Engine/Source/Editor)

