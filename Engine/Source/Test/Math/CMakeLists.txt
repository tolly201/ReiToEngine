cmake_minimum_required(VERSION 3.20)
project(MathTests)

# Auto-discover Math test sources (exclude standalone main)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*_tests.cpp
)

add_library(MathTestsObjs OBJECT ${TEST_SOURCES})

target_include_directories(MathTestsObjs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Runtime
)

# Do not link full runtime here to avoid dragging platform code; prefer header-only math usage
