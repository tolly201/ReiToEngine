# modify_assimp_cmake.cmake

# Path to the Assimp CMakeLists.txt
set(ASSIMP_CMAKELISTS "${CMAKE_SOURCE_DIR}/Engine/Thirdparty/Assimp/CMakeLists.txt")

# Commands to append
set(APPEND_CONTENT "# Dynamically added by ReiTo CMake script.
target_link_libraries(assimp ReiToRuntime)
target_include_directories(assimp PRIVATE
\${CMAKE_SOURCE_DIR}/Engine/Source/Runtime
)")

# Append the content to the Assimp CMakeLists.txt
file(APPEND ${ASSIMP_CMAKELISTS} "${APPEND_CONTENT}")