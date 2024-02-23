cmake_minimum_required(VERSION 3.5.0)
project(OpenMCServer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog)
if (NOT ${spdlog_FOUND})
    message(STATUS "Using custom spdlog")
    add_subdirectory(libs/spdlog)
endif()
add_subdirectory(libs/sockpp)
add_subdirectory(libs/nbtpp)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/world
)
target_link_libraries(${PROJECT_NAME} sockpp spdlog::spdlog nbtpp)