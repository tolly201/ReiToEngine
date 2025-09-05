set(CMAKE_SYSTEM_NAME Darwin)
# 设置编译器路径

set(LLVM_PATH "/usr/local/opt/llvm/bin") # Intel Mac（如需切换）
set(CMAKE_C_COMPILER "${LLVM_PATH}/clang")
set(CMAKE_CXX_COMPILER "${LLVM_PATH}/clang++")
set(CMAKE_LINKER "${LLVM_PATH}/clang++")
# 设置目标平台
set(CMAKE_C_COMPILER_TARGET x86_64-apple-darwin)
set(CMAKE_CXX_COMPILER_TARGET x86_64-apple-darwin)

# 设置构建类型为Debug
set(CMAKE_BUILD_TYPE Debug)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -g -O0")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/lib)



