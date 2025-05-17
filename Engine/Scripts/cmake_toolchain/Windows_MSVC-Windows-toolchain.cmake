# 设置目标系统为 Windows
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(RT_MSVC_PATH "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/bin/Hostx64/x64")

if(NOT DEFINED RT_MSVC_PATH)
    message(FATAL_ERROR "RT_MSVC_PATH is not defined.")
endif()

if (NOT EXISTS ${RT_MSVC_PATH})
    message(FATAL_ERROR "RT_MSVC_PATH does not exist: ${RT_MSVC_PATH}")
endif()

message(STATUS "RT_MSVC_PATH: ${RT_MSVC_PATH}")

# 设置编译器为 MSVC
set(CMAKE_C_COMPILER ${RT_MSVC_PATH}/cl.exe)
set(CMAKE_CXX_COMPILER ${RT_MSVC_PATH}/cl.exe)
# set(CMAKE_RC_COMPILER ${RT_MSVC_PATH}/rc.exe)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc /source-charset:utf-8")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od /MDd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /MD")

# 针对 MSVC 的额外定义
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_definitions(-DUNICODE -D_UNICODE)
# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/lib)  # 静态库
set(CMAKE_OBJECT_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/obj)   # 中间文件
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/)     # exe/dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/lib)  # dll
