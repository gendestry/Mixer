cmake_minimum_required(VERSION 3.10)

# Project setup
project(mixer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect source files
file(GLOB_RECURSE SOURCES
        src/*.cpp
        main.cpp
)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES}
        src/Helper/Curve.h
        src/Command/CommandInterface.h
        src/Command/CommandExecutor.h
        src/Effect/Effect.h)
# Attach include directories to the target (after it's created)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        include
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)
