cmake_minimum_required(VERSION 3.20)
project(MathTests)

# Auto-discover Math test sources
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*_tests.cpp
)

add_executable(MathTests ${TEST_SOURCES})

# Include Runtime headers to reuse math code
target_include_directories(MathTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Runtime
)

# Link against ReiToRuntime to share symbols and compile defs
target_link_libraries(MathTests PRIVATE ReiToRuntime)

# Output next to other runtime bins
set_target_properties(MathTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ReiToEngine/bin/
)

# Register with CTest
add_test(NAME MathTests COMMAND MathTests)
