# remove_dynamic_content.cmake

# Path to the Assimp CMakeLists.txt
set(ASSIMP_CMAKELISTS "${CMAKE_SOURCE_DIR}/Engine/Thirdparty/Assimp/CMakeLists.txt")

# Read the file content
file(READ ${ASSIMP_CMAKELISTS} ORIGINAL_CONTENTS)
message("Original Content:\n${ORIGINAL_CONTENTS}")
string(REGEX REPLACE "(# Dynamically added by ReiTo CMake script.*)" "" MODIFIED_CONTENTS "${ORIGINAL_CONTENTS}")

# Write the modified content back to the file
file(WRITE ${ASSIMP_CMAKELISTS} "${MODIFIED_CONTENTS}")