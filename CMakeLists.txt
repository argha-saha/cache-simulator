# Project Setup
cmake_minimum_required(VERSION 3.28)
project(CacheSimulator VERSION 1.0 LANGUAGES CXX)

# C++ Standard Configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# All source files (.cpp) located in the src directory
file(GLOB SOURCE_FILES "src/*.cpp")

# Define the target
add_executable(CacheSimulator ${SOURCE_FILES})

# Include directories
target_include_directories(CacheSimulator PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

# Output messages
message(STATUS "Configuring Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source Files: ${SOURCE_FILES}")
message(STATUS "Include Directories: ${PROJECT_SOURCE_DIR}/include")