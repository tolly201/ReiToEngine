cmake_minimum_required(VERSION 3.10)
# 自动检测目标平台和编译器
if(NOT DEFINED RT_TARGET_PLATFORM)
    if(WIN32)
        set(RT_TARGET_PLATFORM "Windows")
    elseif(APPLE)
        set(RT_TARGET_PLATFORM "MacOS")
    elseif(UNIX)
        set(RT_TARGET_PLATFORM "Linux")
    else()
        message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
    endif()
endif()

if(NOT DEFINED RT_COMPILE_PLATFORM)
    if(DEFINED ENV{RT_COMPILE_PLATFORM})
        set(RT_COMPILE_PLATFORM $ENV{RT_COMPILE_PLATFORM})
    endif()
endif()

# 根据编译器和平台设置编译环境
if(WIN32)
    if(NOT DEFINED RT_COMPILE_PLATFORM)
        message(WARNING "Default Compiler For Windows: MSVC.")
        set(RT_COMPILE_PLATFORM "Windows_MSVC")
    endif()
elseif(APPLE)
    if(NOT DEFINED RT_COMPILE_PLATFORM)
        message(WARNING "Default Compiler For MacOS: clang.")
        set(RT_COMPILE_PLATFORM "MacOS_CLANG")
    endif()
elseif(UNIX)
    if(NOT DEFINED RT_COMPILE_PLATFORM)
        message(WARNING "Default Compiler For Linux: clang.")
        set(RT_COMPILE_PLATFORM "Linux_CLANG")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

message(STATUS "Compile Platform: ${RT_COMPILE_PLATFORM}")
message(STATUS "Target Platform: ${RT_TARGET_PLATFORM}")

# 加载工具链文件
set(TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/Engine/Scripts/cmake_toolchain/${RT_COMPILE_PLATFORM}-${RT_TARGET_PLATFORM}-toolchain.cmake")
if(EXISTS ${TOOLCHAIN_FILE})
    include(${TOOLCHAIN_FILE})
else()
    message(FATAL_ERROR "Toolchain file not found: ${TOOLCHAIN_FILE}")
endif()

project(ReiToEngine)
# 通用编译设置
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 针对不同编译器的额外设置
if(MSVC)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# 资源文件输出目录
set(RESOURCE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/res)

# 引擎目录设置
set(ENGINE_ROOT_DIR Engine)

# 包含第三方库和引擎代码
include_directories(
    ${ENGINE_ROOT_DIR}/Thirdparty/stb/
)
add_subdirectory(Engine/Source/Runtime)

# 设置 Assimp 的构建选项
set(ASSIMP_BUILD_TESTS OFF)        # 禁用测试
set(ASSIMP_BUILD_SAMPLES OFF)      # 禁用示例
set(ASSIMP_BUILD_DOCS OFF)         # 禁用文档
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
include_directories(
    ${ENGINE_ROOT_DIR}/Thirdparty/Assimp/include/
)
add_subdirectory(Engine/Thirdparty/Assimp)
add_subdirectory(Engine/Source/Editor)

