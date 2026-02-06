cmake_minimum_required(VERSION 3.10)

# Project setup
project(mixer VERSION 1.0 LANGUAGES CXX)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --------------------------------------------------
# Fetch fmt
# --------------------------------------------------
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 12.1.0
)

FetchContent_MakeAvailable(fmt)

# --------------------------------------------------
# Your target
# --------------------------------------------------

# Collect source files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
        PRIVATE fmt::fmt
)

# Attach include directories to the target (after it's created)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        include
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
