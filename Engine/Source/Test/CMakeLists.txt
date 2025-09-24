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

# 直接加入 Math 测试源（保持与 Core 平行的 Math 目录）
file(GLOB TEST_MATH_SOURCES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/Math/*_tests.cpp
)

file(GLOB TEST_MEMORY_SOURCES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/Memory/*_tests.cpp
)
file(GLOB TEST_CONTAINER_SOURCES CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/Containers/*_tests.cpp
)

target_sources(ReiToTests PRIVATE ${TEST_MATH_SOURCES} ${TEST_MEMORY_SOURCES} ${TEST_CONTAINER_SOURCES})

# 直接链接完整运行时，而不是复制其源文件，确保平台实现与实际运行时保持一致
target_link_libraries(ReiToTests PRIVATE ReiToRuntime)

set_target_properties(ReiToTests PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/bin/
)

add_test(NAME AllTests COMMAND ReiToTests)
