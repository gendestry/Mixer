cmake_minimum_required(VERSION 3.10)

project(lucke VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the library folder
add_subdirectory(lib)

# Add main executable
add_executable(${PROJECT_NAME}_app main.cpp)

# Link the library
target_link_libraries(${PROJECT_NAME}_app PRIVATE lucke_lib)

# Include directories for the executable (if needed)
target_include_directories(${PROJECT_NAME}_app PRIVATE lib/include)