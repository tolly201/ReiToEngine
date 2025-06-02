# 设置目标系统为 Windows
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(RT_MSYS2_PATH "D:/local/software/MSYS2")

if(NOT DEFINED RT_MSYS2_PATH)
    message(FATAL_ERROR "RT_MSYS2_PATH is not defined.")
endif()

if (NOT EXISTS ${RT_MSYS2_PATH})
    message(FATAL_ERROR "RT_MSYS2_PATH does not exist: ${RT_MSYS2_PATH}")
endif()

message(STATUS "RT_MSYS2_PATH: ${RT_MSYS2_PATH}")

if (NOT EXISTS ${RT_MSYS2_PATH}/clang64)
    message(FATAL_ERROR "RT_MSYS2_PATH clang does not exist: ${RT_MSYS2_PATH}/clang64")
endif()


# 设置编译器路径
set(CMAKE_C_COMPILER "${RT_MSYS2_PATH}/clang64/bin/clang.exe")
set(CMAKE_CXX_COMPILER "${RT_MSYS2_PATH}/clang64/bin/clang++.exe")
set(CMAKE_RC_COMPILER "${RT_MSYS2_PATH}/clang64/bin/windres.exe")
set(CMAKE_AR "${RT_MSYS2_PATH}/clang64/bin/ar.exe")
set(CMAKE_RANLIB "${RT_MSYS2_PATH}/clang64/bin/ranlib.exe")
set(CMAKE_MAKE_PROGRAM "${RT_MSYS2_PATH}/usr/bin/make.exe")

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_BUILD_TYPE Debug)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/lib)  # 静态库
set(CMAKE_OBJECT_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/obj)   # 中间文件
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/)     # exe/dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/lib)  # dll


add_definitions(-DUNICODE -D_UNICODE)
