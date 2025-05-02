# Project Setup
cmake_minimum_required(VERSION 3.28)
project(CacheSimulator VERSION 1.0 LANGUAGES CXX)

# C++ Standard Configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# GoogleTest Integration
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
# For Windows:
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# All source files (.cpp) located in the src directory
file(GLOB LIB_SOURCE_FILES
    CONFIGURE_DEPENDS # Re-run cmake if files are added/removed
    "src/*.cpp"
    EXCLUDE "src/main.cpp"
)

# Define the core library target
add_library(CacheSimulatorLib ${LIB_SOURCE_FILES})

# Make include directories available to targets linking against this library
target_include_directories(CacheSimulatorLib
    PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
)

# Define the main application executable
add_executable(CacheSimulator src/main.cpp)

# Link the core library to the main application
target_link_libraries(CacheSimulator PRIVATE CacheSimulatorLib)

# Enable testing with GoogleTest
enable_testing()
include(GoogleTest)

# Gather test source files
file(GLOB TEST_SOURCE_FILES
    CONFIGURE_DEPENDS
    "tests/*.cpp"
)

# Define the test executable target
add_executable(CacheSimulatorTests ${TEST_SOURCE_FILES})

# Link the test executable against the core library and GoogleTest
target_link_libraries(CacheSimulatorTests
    PRIVATE
        CacheSimulatorLib  # Link against the code being tested
        GTest::gtest_main  # Links gtest & provides default main() for tests
)

gtest_discover_tests(CacheSimulatorTests)

# Output messages
message(STATUS "Configuring Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source Files: ${SOURCE_FILES}")
message(STATUS "Include Directories: ${PROJECT_SOURCE_DIR}/include")