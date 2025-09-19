cmake_minimum_required(VERSION 3.20)
project(ReiToEngineTests)

# Each module under Engine/Source/Test can have its own CMake; for now we include Math
add_subdirectory(Math)
