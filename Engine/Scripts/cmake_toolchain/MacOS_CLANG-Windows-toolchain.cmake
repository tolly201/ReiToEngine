set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
# 设置编译器路径
set(RT_MINGW_PATH /opt/local)
if(NOT DEFINED RT_MINGW_PATH)
    message(FATAL_ERROR "RT_MINGW_PATH is not defined.")
endif()

if (NOT EXISTS ${RT_MINGW_PATH})
    message(FATAL_ERROR "RT_MINGW_PATH does not exist: ${RT_MINGW_PATH}")
endif()

message(STATUS "RT_MINGW_PATH: ${RT_MINGW_PATH}")

# 设置编译器路径
set(CMAKE_C_COMPILER ${RT_MINGW_PATH}/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER ${RT_MINGW_PATH}/bin/x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER ${RT_MINGW_PATH}/bin/x86_64-w64-mingw32-windres)
# 使用 LLD 作为链接器
set(CMAKE_LINKER ${RT_MINGW_PATH}/bin/x86_64-w64-mingw32-ld)

# 显式设置 include 和 library 路径
include_directories(${RT_MINGW_PATH}/x86_64-w64-mingw32/include)
link_directories(${RT_MINGW_PATH}/x86_64-w64-mingw32/lib)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcc -lstdc++")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lgcc -lstdc++")
