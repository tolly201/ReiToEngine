cmake_minimum_required(VERSION 3.10)
project(ReiToEditor)


file(GLOB_RECURSE EDITOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/**.cpp)
file(GLOB_RECURSE EDITOR_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/**.h)
# file(GLOB PROJECT_SHADERS Engine/Shaders/*.glsl) # 假设你的shader文件扩展名为 .glsl

# 构建使用 DLL 的可执行文件
add_executable(ReiToEditor ${EDITOR_SOURCES})
# 
target_link_libraries(ReiToEditor assimp ReiToRuntime )
# target_link_libraries(ReiToEditor )
# Set compile flags for ReiToRuntime (important for custom delete)

target_include_directories(ReiToEditor PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/../Runtime 
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../../Thirdparty
) # editor 需要包含 runtime 和 editor 的头文件

# 假设你使用glslang来编译shader，这里只是一个示例，你需要根据实际情况修改
# find_package(glslang REQUIRED)
# add_custom_command(...) # 使用glslang编译shader