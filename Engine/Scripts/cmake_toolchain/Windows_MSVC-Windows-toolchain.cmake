# 设置目标系统为 Windows
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(RT_MSVC_PATH "C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64")

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
