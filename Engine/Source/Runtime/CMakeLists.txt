cmake_minimum_required(VERSION 3.10)
project(ReiToRuntime)

file(GLOB_RECURSE RUNTIME_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/**.cpp)
file(GLOB_RECURSE RUNTIME_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/**.h)
# 构建 DLL
add_library(ReiToRuntime SHARED)
target_sources(ReiToRuntime PRIVATE
                ${RUNTIME_SOURCES}
)
# Set compile flags for ReiToRuntime (important for custom delete)
# set_target_properties(ReiToRuntime PROPERTIES COMPILE_FLAGS)

target_compile_definitions(ReiToRuntime PRIVATE REITOENGINE_EXPORTS)
target_include_directories(ReiToRuntime PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../../Thirdparty/stb/
)

# 创建导出目标列表
install(TARGETS ReiToRuntime
    EXPORT ReiToRuntimeTargets
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ReiToRuntime # 导出到构建目录下的子目录
)

# 创建并导出 CMake 配置文件
install(EXPORT ReiToRuntimeTargets
    FILE ReiToRuntimeTargets.cmake
    NAMESPACE ReiToRuntime:: # 添加命名空间，避免命名冲突
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ReiToRuntime/cmake
)
