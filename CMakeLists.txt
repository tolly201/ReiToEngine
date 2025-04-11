cmake_minimum_required(VERSION 3.10)
project(ReiToEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /utf-8")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -g -O0")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -g -pg -O0")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/lib)  # 静态库
set(CMAKE_OBJECT_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/obj)   # 中间文件
if(MSVC)
    # Windows: 所有运行时文件统一到bin
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/)  # exe/dll
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/lib)   # dll
else()
    # Unix-like: 分离可执行文件和库文件
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine)  # exe
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/lib)  # so/dylib
endif()

# 资源文件目录
set(RESOURCE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/res)

set(ENGINE_ROOT_DIR Engine)

# set(ASSIMP_NO_EXPORT ON)           # 如果不需要导出功能，可以禁用
include_directories(
# ${ENGINE_ROOT_DIR}Thirdparty/glad/include/
${ENGINE_ROOT_DIR}/Thirdparty/stb/
# ${ENGINE_ROOT_DIR}Thirdparty/Assimp/include/
)
add_subdirectory(Engine/Source/Runtime)

# 设置 Assimp 的构建选项
set(ASSIMP_BUILD_TESTS OFF)        # 禁用测试
set(ASSIMP_BUILD_SAMPLES OFF)      # 禁用示例
set(ASSIMP_BUILD_DOCS OFF)         # 禁用文档
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
include(${CMAKE_SOURCE_DIR}/Engine/Scripts/modify_cmake.cmake)
include_directories(
# ${ENGINE_ROOT_DIR}Thirdparty/stb/
${ENGINE_ROOT_DIR}Thirdparty/Assimp/include/
)
add_subdirectory(Engine/Thirdparty/Assimp)
add_subdirectory(Engine/Source/Editor)

include(${CMAKE_SOURCE_DIR}/Engine/Scripts/remove_modify.cmake)
