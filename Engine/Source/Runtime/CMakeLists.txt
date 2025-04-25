cmake_minimum_required(VERSION 3.10)
project(ReiToRuntime)

file(GLOB_RECURSE RUNTIME_SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/**.cpp
${CMAKE_CURRENT_SOURCE_DIR}/**.mm
)
file(GLOB_RECURSE RUNTIME_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/**.h)

add_custom_target(ClangD_Runtime_Header_Support SOURCES ${RUNTIME_HEADERS})
# 构建 DLL
add_library(ReiToRuntime SHARED)
target_sources(ReiToRuntime PRIVATE
                ${RUNTIME_SOURCES}
)
# Set compile flags for ReiToRuntime (important for custom delete)
# set_target_properties(ReiToRuntime PROPERTIES COMPILE_FLAGS)

if(MSVC)
    target_compile_options(ReiToRuntime PRIVATE /RTC1 /sdl)
    target_compile_options(ReiToRuntime PRIVATE
    -ObjC++
    )
elseif(APPLE)
    # GCC/Clang 编译选项
    target_compile_options(ReiToRuntime PRIVATE
        -ggdb         # 生成GDB调试信息
        -g            # 生成调试信息
        -O0           # 禁用优化
        -fsanitize=address
        -fno-omit-frame-pointer
    )
    target_link_options(ReiToRuntime PRIVATE
        -fsanitize=address
    )
    target_compile_options(ReiToRuntime PRIVATE
    -ObjC++
    )

    target_link_libraries(ReiToRuntime
        "-framework Foundation"
        "-framework Cocoa"
    )
else()
    # GCC/Clang 编译选项
    target_compile_options(ReiToRuntime PRIVATE
        -ggdb         # 生成GDB调试信息
        -g            # 生成调试信息
        -pg           # 生成性能分析信息（用于gprof）
        -O0           # 禁用优化
        -fsanitize=address
        -fno-omit-frame-pointer
    )
    target_link_options(ReiToRuntime PRIVATE
        -pg           # 链接时也需要添加性能分析选项
        -fsanitize=address
    )
    target_compile_options(ReiToRuntime PRIVATE
    -ObjC++
    )
endif()

target_compile_definitions(ReiToRuntime PRIVATE RT_EXPORTS)
target_include_directories(ReiToRuntime PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../../Thirdparty/stb/
${CMAKE_CURRENT_SOURCE_DIR}/../../Thirdparty/Assimp/include
)

# 创建导出目标列表
install(TARGETS ReiToRuntime
    EXPORT ReiToRuntimeTargets
    DESTINATION ${CMAKE_BINARY_DIR}/ReiToEngine/lib # 导出到构建目录下的子目录
)

# 创建并导出 CMake 配置文件
install(EXPORT ReiToRuntimeTargets
    FILE ReiToRuntimeTargets.cmake
    NAMESPACE ReiToRuntime:: # 添加命名空间，避免命名冲突
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/ReiToRuntime/cmake
)
