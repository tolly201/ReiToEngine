cmake_minimum_required(VERSION 3.20)
set(CMAKE_OSX_DEPLOYMENT_TARGET "13.7" CACHE STRING "Minimum macOS deployment version" FORCE)
project(ReiToEngine)
# 通用编译设置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 统一输出目录：如果在 Presets 中定义了 CMAKE_PROJECT_SOURCE_OUTPUT_DIRECTORY，则在此派生 bin/lib 目录
if(DEFINED CMAKE_PROJECT_SOURCE_OUTPUT_DIRECTORY AND NOT CMAKE_PROJECT_SOURCE_OUTPUT_DIRECTORY STREQUAL "")
  # 规范化路径分隔符
  file(TO_CMAKE_PATH "${CMAKE_PROJECT_SOURCE_OUTPUT_DIRECTORY}" _RT_PROJ_OUTDIR)
  # 单配置生成器（Ninja 等）
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${_RT_PROJ_OUTDIR}/bin")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${_RT_PROJ_OUTDIR}/lib")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${_RT_PROJ_OUTDIR}/lib")
  # 多配置生成器（MSVC 等）
  foreach(_cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_cfg} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${_cfg} "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${_cfg} "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
  endforeach()
endif()


# 针对不同编译器的额外设置
if(MSVC)
  # Enforce standard conforming preprocessor and C++20 (guard in case of generator overriding)
  add_compile_options(/Zc:preprocessor)
else()
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
endif()

find_package(Vulkan REQUIRED)
# 资源文件输出目录
set(RESOURCE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/assets)
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

set(MI_OVERRIDE ON CACHE BOOL "" FORCE)
set(MI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MI_BUILD_SHARED ON CACHE BOOL "" FORCE)
set(MI_BUILD_STATIC ON  CACHE BOOL "" FORCE)
add_subdirectory(Engine/Thirdparty/mimalloc)

add_subdirectory(Engine/Source/Runtime)

# 设置 Assimp 的构建选项
set(ASSIMP_BUILD_TESTS OFF)        # 禁用测试
set(ASSIMP_BUILD_SAMPLES OFF)      # 禁用示例
set(ASSIMP_BUILD_DOCS OFF)         # 禁用文档
set(ASSIMP_WARNINGS_AS_ERRORS OFF)

# add_subdirectory(Engine/Thirdparty/Assimp)
add_subdirectory(Engine/Source/Editor)

# ---------------------- Tests (no third-party) ----------------------
option(RT_ENABLE_TESTS "Build internal test targets" OFF)
if(RT_ENABLE_TESTS)
    include(CTest)
    add_subdirectory(Engine/Source/Test)
endif()

# ---------------------- Shader pipeline (glslc) ----------------------
# 1) 定位 glslc
find_program(GLSLC glslc HINTS "$ENV{VULKAN_SDK}/Bin" "$ENV{VULKAN_SDK}/bin")
if(NOT GLSLC)
  message(FATAL_ERROR "glslc not found. Please set VULKAN_SDK or add glslc to PATH.")
endif()

# 2) 目录设置
set(RT_SHADER_SRC_DIR "${CMAKE_SOURCE_DIR}/assets/shaders")
# 若已通过 preset 设置了 CMAKE_RUNTIME_OUTPUT_DIRECTORY，这里复用；否则兜底
if(CMAKE_PROJECT_SOURCE_OUTPUT_DIRECTORY)
  set(RT_SHADER_OUT_DIR "${CMAKE_PROJECT_SOURCE_OUTPUT_DIRECTORY}/assets/shaders")
else()
  set(RT_SHADER_OUT_DIR "${CMAKE_BINARY_DIR}/ReiToEngine/assets/shaders")
endif()

# 3) 收集着色器并生成对应的 .spv 规则（支持递归）
file(GLOB_RECURSE RT_SHADER_SOURCES
  "${RT_SHADER_SRC_DIR}/*.vert.glsl"
  "${RT_SHADER_SRC_DIR}/*.frag.glsl"
  "${RT_SHADER_SRC_DIR}/*.comp.glsl"
)

set(RT_SHADER_OUTPUTS "")
foreach(SRC ${RT_SHADER_SOURCES})
  file(RELATIVE_PATH REL "${RT_SHADER_SRC_DIR}" "${SRC}")
  set(SPV_REL "${REL}")
  string(REGEX REPLACE "\\.(vert|frag|comp)\\.glsl$" ".spv" SPV_REL "${SPV_REL}")
  set(OUT "${RT_SHADER_OUT_DIR}/${SPV_REL}")

  # 推断 stage
  set(STAGE "")
  if(SRC MATCHES "\\.vert(\\.glsl)?$")      # 顶点
    set(STAGE "vert")
  elseif(SRC MATCHES "\\.frag(\\.glsl)?$")  # 片元
    set(STAGE "frag")
  elseif(SRC MATCHES "\\.comp(\\.glsl)?$")  # 计算
    set(STAGE "comp")
  endif()

  add_custom_command(
    OUTPUT "${OUT}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${RT_SHADER_OUT_DIR}"
    COMMAND "${GLSLC}" $<$<BOOL:${STAGE}>:-fshader-stage=${STAGE}> "${SRC}" -o "${OUT}"
    DEPENDS "${SRC}"
    COMMENT "glslc ${REL} -> ${SPV_REL}"
    VERBATIM
  )
  list(APPEND RT_SHADER_OUTPUTS "${OUT}")
endforeach()

# 4) 聚合为一个目标，并让运行期目标依赖它
if(RT_SHADER_OUTPUTS)
  add_custom_target(shaders DEPENDS ${RT_SHADER_OUTPUTS})
  # 你的可执行/库可能在子目录里定义，按存在性绑定依赖
  if(TARGET ReiToRuntime)
    add_dependencies(ReiToRuntime shaders)
  endif()
  if(TARGET ReiToEditor)
    add_dependencies(ReiToEditor shaders)
  endif()
endif()
