# 设置目标系统为 Windows
set(CMAKE_SYSTEM_NAME Windows)

set(RT_LLVM_PATH "C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/Llvm")

if(NOT DEFINED RT_LLVM_PATH)
    message(FATAL_ERROR "RT_LLVM_PATH is not defined.")
endif()

if (NOT EXISTS ${RT_LLVM_PATH})
    message(FATAL_ERROR "RT_LLVM_PATH does not exist: ${RT_LLVM_PATH}")
endif()

message(STATUS "RT_LLVM_PATH: ${RT_LLVM_PATH}")

if (NOT EXISTS ${RT_LLVM_PATH}/bin)
    message(FATAL_ERROR "RT_LLVM_PATH bin does not exist: ${RT_LLVM_PATH}/clang64")
endif()


# 设置编译器路径
set(CMAKE_C_COMPILER "${RT_LLVM_PATH}/bin/clang.exe")
set(CMAKE_CXX_COMPILER "${RT_LLVM_PATH}/bin/clang++.exe")
set(CMAKE_RC_COMPILER "${RT_LLVM_PATH}/bin/llvm-rc.exe")
set(CMAKE_AR "${RT_LLVM_PATH}/bin/llvm-ar.exe")
set(CMAKE_RANLIB "${RT_LLVM_PATH}/bin/llvm-ranlib.exe")
# set(CMAKE_MAKE_PROGRAM "${RT_LLVM_PATH}/usr/bin/make.exe")
