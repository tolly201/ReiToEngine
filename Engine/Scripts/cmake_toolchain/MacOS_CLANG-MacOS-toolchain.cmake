set(CMAKE_SYSTEM_NAME Darwin)
# 设置编译器路径

set(LLVM_PATH "/usr/bin") # Intel Mac（如需切换）
# set(LLD_PATH "/usr/bin") # Intel Mac（如需切换）
set(CMAKE_C_COMPILER "${LLVM_PATH}/clang")
set(CMAKE_CXX_COMPILER "${LLVM_PATH}/clang++")


# custom linkers only when using brew installed llvm on mac silicon
# lld 路径设置（只在需要 lld 时生效）
# set(CMAKE_EXE_LINKER_FLAGS   "${CMAKE_EXE_LINKER_FLAGS} --ld-path=${LLD_PATH}/ld64.lld")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --ld-path=${LLD_PATH}/ld64.lld")
# set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --ld-path=${LLD_PATH}/ld64.lld")
# 设置目标平台
set(CMAKE_C_COMPILER_TARGET x86_64-apple-darwin)
set(CMAKE_CXX_COMPILER_TARGET x86_64-apple-darwin)
