cmake_minimum_required(VERSION 3.20)
project(ReiToEngineTests)

# Core test framework and runner (module-independent)
add_executable(ReiToTests
	Core/main.cpp
    Core/RuntimeTestInit.cpp
)

target_include_directories(ReiToTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Keep tests independent of full Runtime by default. If a module needs linking, add it per-module.

# Math tests as object library
add_subdirectory(Math)

target_sources(ReiToTests PRIVATE
	$<TARGET_OBJECTS:MathTestsObjs>
	# 最小 Runtime 实现以满足 ABI
	${CMAKE_CURRENT_SOURCE_DIR}/../Runtime/L20_Platform/L31_SingletonFactory/SingletonFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../Runtime/L20_Platform/L30_MemoryManager/RTMimallocManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../Runtime/L20_Platform/L21_Console/Windows.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../Runtime/L20_Platform/L20_Memory/Windows.cpp
)

# 导出宏以避免 __declspec(dllimport) 在本目标内定义符号
target_compile_definitions(ReiToTests PRIVATE RT_EXPORTS)

# mimalloc 依赖（供 RTMimallocManager 使用）
target_link_libraries(ReiToTests PRIVATE mimalloc-static)
target_include_directories(ReiToTests PRIVATE ${ENGINE_ROOT_DIR}/Thirdparty/mimalloc/include)

set_target_properties(ReiToTests PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/bin/
)

add_test(NAME AllTests COMMAND ReiToTests)
