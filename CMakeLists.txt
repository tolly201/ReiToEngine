cmake_minimum_required(VERSION 3.10)
project(ReiToEngine)

set(CMAKE_CXX_STANDARD 26)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

set(ENGINE_ROOT_DIR Engine)

# set(ASSIMP_NO_EXPORT ON)           # 如果不需要导出功能，可以禁用
include_directories(
# ${ENGINE_ROOT_DIR}Thirdparty/glad/include/
${ENGINE_ROOT_DIR}Thirdparty/stb/
# ${ENGINE_ROOT_DIR}Thirdparty/Assimp/include/
)
set (ENGINE_RUNTIME_DIR Engine/Source/Runtime)
add_subdirectory(Engine/Source/Runtime)

# 设置 Assimp 的构建选项
set(ASSIMP_BUILD_TESTS OFF)        # 禁用测试
set(ASSIMP_BUILD_SAMPLES OFF)      # 禁用示例
set(ASSIMP_BUILD_DOCS OFF)         # 禁用文档
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
include(${CMAKE_SOURCE_DIR}/Engine/Scripts/modify_cmake.cmake)
include_directories(
# ${ENGINE_ROOT_DIR}Thirdparty/glad/include/
# ${ENGINE_ROOT_DIR}Thirdparty/stb/
${ENGINE_ROOT_DIR}Thirdparty/Assimp/include/
)
add_subdirectory(Engine/Thirdparty/Assimp)

add_subdirectory(Engine/Source/Editor)
include(${CMAKE_SOURCE_DIR}/Engine/Scripts/remove_modify.cmake)