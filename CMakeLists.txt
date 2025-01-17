cmake_minimum_required(VERSION 3.10)
project(ReiToEngine)

set(CMAKE_CXX_STANDARD 26)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()
# 定义源文件目录 - 更清晰的组织方式
set(RUNTIME_SOURCE_DIR Engine/Source/Runtime)
set(EDITOR_SOURCE_DIR Engine/Source/Editor)

file(GLOB_RECURSE RUNTIME_SOURCES Engine/Source/Runtime/**.cpp)
file(GLOB_RECURSE RUNTIME_HEADERS Engine/Source/Runtime/**.h)
file(GLOB_RECURSE EDITOR_SOURCES Engine/Source/Editor/**.cpp)
file(GLOB_RECURSE EDITOR_HEADERS Engine/Source/Editor/**.h)
# file(GLOB PROJECT_SHADERS Engine/Shaders/*.glsl) # 假设你的shader文件扩展名为 .glsl

source_group("Headers" FILES ${RUNTIME_HEADERS})
source_group("Headers" FILES ${EDITOR_HEADERS})
# source_group("Shaders" FILES ${PROJECT_SHADERS})
# 构建 DLL
add_library(${PROJECT_NAME} SHARED)
target_sources( ReiToEngine PRIVATE
                ${RUNTIME_SOURCES}
)
target_compile_definitions(${PROJECT_NAME} PRIVATE REITOENGINE_EXPORTS)

# 构建使用 DLL 的可执行文件
add_executable(editor ${EDITOR_SOURCES})
target_link_libraries(editor ReiToEngine)

# 可选择: 设置包含目录
target_include_directories(ReiToEngine PUBLIC ${RUNTIME_SOURCE_DIR})
target_include_directories(editor PRIVATE ${EDITOR_SOURCE_DIR} ${RUNTIME_SOURCE_DIR}) # editor 需要包含 runtime 和 editor 的头文件


# 假设你使用glslang来编译shader，这里只是一个示例，你需要根据实际情况修改
# find_package(glslang REQUIRED)
# add_custom_command(...) # 使用glslang编译shader