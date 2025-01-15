cmake_minimum_required(VERSION 3.10)
project(ReiToEngine)

set(CMAKE_CXX_STANDARD 26)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

file(GLOB_RECURSE PROJECT_SOURCES Engine/Source/**.cpp)
file(GLOB_RECURSE PROJECT_HEADERS Engine/Source/**.h)
# file(GLOB PROJECT_SHADERS Engine/Shaders/*.glsl) # 假设你的shader文件扩展名为 .glsl

source_group("Headers" FILES ${PROJECT_HEADERS})
# source_group("Shaders" FILES ${PROJECT_SHADERS})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# 假设你使用glslang来编译shader，这里只是一个示例，你需要根据实际情况修改
# find_package(glslang REQUIRED)
# add_custom_command(...) # 使用glslang编译shader

# 如果需要链接库
# target_link_libraries(ReiToEngine your_library) 