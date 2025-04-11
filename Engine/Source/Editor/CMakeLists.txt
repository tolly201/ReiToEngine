cmake_minimum_required(VERSION 3.10)
project(ReiToEditor)


file(GLOB_RECURSE EDITOR_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/**.cpp)
file(GLOB_RECURSE EDITOR_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/**.h)
# file(GLOB PROJECT_SHADERS Engine/Shaders/*.glsl) # 假设你的shader文件扩展名为 .glsl

# 构建使用 DLL 的可执行文件
add_executable(ReiToEditor ${EDITOR_SOURCES})
#

if(MSVC)
    target_compile_options(ReiToEditor PRIVATE /RTC1 /sdl)
    target_compile_options(ReiToEditor PRIVATE
    -ObjC++
    )
elseif(APPLE)
    # GCC/Clang 编译选项
    target_compile_options(ReiToEditor PRIVATE
        -ggdb         # 生成GDB调试信息
        -g            # 生成调试信息
        -O0           # 禁用优化
        -fsanitize=address
        -fno-omit-frame-pointer
    )
    target_link_options(ReiToEditor PRIVATE
        -fsanitize=address
    )
    target_compile_options(ReiToEditor PRIVATE
    -ObjC++
    )

    set(INFO_PLIST_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)

    set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${INFO_PLIST_FILE}
    RESOURCE "${EDITOR_SOURCES}"
)

else()
    # GCC/Clang 编译选项
    target_compile_options(ReiToEditor PRIVATE
        -ggdb         # 生成GDB调试信息
        -g            # 生成调试信息
        -pg           # 生成性能分析信息（用于gprof）
        -O0           # 禁用优化
        -fsanitize=address
        -fno-omit-frame-pointer
    )
    target_link_options(ReiToEditor PRIVATE
        -pg           # 链接时也需要添加性能分析选项
        -fsanitize=address
    )
    target_compile_options(ReiToEditor PRIVATE
    -ObjC++
    )
endif()

target_link_libraries(ReiToEditor assimp ReiToRuntime )
# target_link_libraries(ReiToEditor )
# Set compile flags for ReiToRuntime (important for custom delete)

target_include_directories(ReiToEditor PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/../Runtime
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/../../Thirdparty/stb
${CMAKE_CURRENT_SOURCE_DIR}/../../Thirdparty/Assimp/include
) # editor 需要包含 runtime 和 editor 的头文件

# 假设你使用glslang来编译shader，这里只是一个示例，你需要根据实际情况修改
# find_package(glslang REQUIRED)
# add_custom_command(...) # 使用glslang编译shader
