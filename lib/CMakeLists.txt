cmake_minimum_required(VERSION 3.10)

project(lucke_lib VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE LIB_SOURCES
	src/**/*.cpp
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCES}
		include/DMXOutput/Outputable.h)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		${PROJECT_SOURCE_DIR}/include
	PRIVATE
		${PROJECT_SOURCE_DIR}/src
)


target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)


