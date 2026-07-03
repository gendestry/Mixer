cmake_minimum_required(VERSION 3.16)

# Project setup
project(LightEngine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib/Utils)

# --------------------------------------------------
# LightEngine library
# --------------------------------------------------
file(GLOB_RECURSE LIGHTENGINE_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(LightEngine ${LIGHTENGINE_SOURCES})
add_library(LightEngine::LightEngine ALIAS LightEngine)

target_compile_features(LightEngine PUBLIC cxx_std_23)
# Utils types appear in public headers (IP, RGB, FragmentedStorage) -> PUBLIC.
target_link_libraries(LightEngine PUBLIC utils)
target_include_directories(LightEngine
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# --------------------------------------------------
# Example / demo app (was the old executable)
# --------------------------------------------------
add_executable(LightEngine_demo examples/demo/main.cpp)
target_link_libraries(LightEngine_demo PRIVATE LightEngine::LightEngine)
