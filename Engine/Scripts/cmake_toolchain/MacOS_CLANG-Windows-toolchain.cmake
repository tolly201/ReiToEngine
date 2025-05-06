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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/lib)  # 动态库
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/lib)  # 静态库

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fseh-exceptions")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fseh-exceptions")

# 显式设置 include 和 library 路径
include_directories(${RT_MINGW_PATH}/x86_64-w64-mingw32/include)
link_directories(${RT_MINGW_PATH}/x86_64-w64-mingw32/lib)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcc -lstdc++")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lgcc -lstdc++")
